package main

import (
	"archive/tar"
	"archive/zip"
	"context"
	"io"
	"log"
	"net/http"
	"os"
	"path"
	"path/filepath"
	"strings"

	"github.com/aws/aws-sdk-go-v2/aws"
	v4 "github.com/aws/aws-sdk-go-v2/aws/signer/v4"
	"github.com/aws/aws-sdk-go-v2/config"
	"github.com/aws/aws-sdk-go-v2/service/s3"
	"github.com/spf13/cobra"
	"github.com/xi2/xz"
)

type S3PresignGetObjectAPI interface {
	PresignGetObject(
		ctx context.Context,
		params *s3.GetObjectInput,
		optFns ...func(*s3.PresignOptions)) (*v4.PresignedHTTPRequest, error)
}

func GetPresignedURL(c context.Context, api S3PresignGetObjectAPI, input *s3.GetObjectInput) (*v4.PresignedHTTPRequest, error) {
	return api.PresignGetObject(c, input)
}

func extract_xz(src string, dst string) {
	fp, err := os.Open(src)
	if err != nil {
		log.Fatal(err)
	}

	reader, err := xz.NewReader(fp, 0)
	if err != nil {
		log.Fatal(err)
	}

	tarReader := tar.NewReader(reader)
	for {
		hdr, err := tarReader.Next()
		if err == io.EOF {
			break
		}

		switch hdr.Typeflag {
		case tar.TypeDir:
			//fmt.Println("mkdir " + path.Join(dst, hdr.Name))
			err = os.MkdirAll(filepath.ToSlash(path.Join(dst, hdr.Name)), 0777)
			if err != nil {
				log.Fatal(err)
			}
		case tar.TypeReg, tar.TypeRegA:
			fn := path.Join(dst, hdr.Name)
			err = os.MkdirAll(path.Dir(fn), 0777)
			if err != nil {
				log.Fatal(err)
			}
			//fmt.Println("mkfile " + path.Join(dst, hdr.Name))
			w, err := os.Create(filepath.ToSlash(path.Join(dst, hdr.Name)))
			if err != nil {
				log.Fatal(err)
			}
			defer w.Close()

			_, err = io.Copy(w, tarReader)
			if err != nil {
				log.Fatal(err)
			}
		}
	}
}

func extract_zip(src string, dst string) {
	r, err := zip.OpenReader(filepath.ToSlash(src))
	if err != nil {
		log.Fatal(err)
	}
	defer r.Close()

	os.MkdirAll(dst, 0777)

	extractAndWriteFile := func(f *zip.File) error {
		rc, err := f.Open()
		if err != nil {
			log.Fatal(err)
		}
		defer func() {
			if err := rc.Close(); err != nil {
				panic(err)
			}
		}()

		path := filepath.ToSlash(path.Join(dst, f.Name))

		// Check for ZipSlip (Directory traversal)
		if !strings.HasPrefix(path, filepath.ToSlash(filepath.Clean(dst))+string(os.PathSeparator)) {
			log.Fatalf("illegal file path: %s", path)
		}

		if f.FileInfo().IsDir() {
			os.MkdirAll(path, 0777)
		} else {
			os.MkdirAll(filepath.Dir(path), 0777)
			f, err := os.OpenFile(path, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0777)
			if err != nil {
				log.Fatal(err)
			}
			defer f.Close()

			_, err = io.Copy(f, rc)
			if err != nil {
				log.Fatal(err)
			}
		}
		return nil
	}

	for _, f := range r.File {
		err := extractAndWriteFile(f)
		if err != nil {
			log.Fatal(err)
		}
	}
}

var pullCmd = &cobra.Command{
	Use:   "pull",
	Short: "get version",
	Long:  "version",
	Run: func(cmd *cobra.Command, args []string) {
		cfg, err := config.LoadDefaultConfig(context.TODO())
		if err != nil {
			log.Fatal(err)
		}

		// Create an Amazon S3 service client
		client := s3.NewFromConfig(cfg)

		bucket := aws.String("mode7-assets")
		key := aws.String("assets.zip")

		input := &s3.GetObjectInput{
			Bucket: bucket,
			Key:    key,
		}

		psClient := s3.NewPresignClient(client)

		resp, err := GetPresignedURL(context.TODO(), psClient, input)
		if err != nil {
			log.Fatal(err)
		}

		httpresp, err := http.Get(resp.URL)
		if err != nil {
			log.Fatal(err)
		}
		defer httpresp.Body.Close()

		out, err := os.Create("./assets.zip")
		if err != nil {
			log.Fatal(err)
		}
		defer out.Close()

		_, err = io.Copy(out, httpresp.Body)
		if err != nil {
			log.Fatal(err)
		}

		extract_zip("./assets.zip", "../../assets")
		os.Remove("./assets.zip")
	},
}
