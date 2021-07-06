package main

import (
	"archive/tar"
	"context"
	"io"
	"log"
	"net/http"
	"os"
	"path"

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
			err = os.MkdirAll(path.Join(dst, hdr.Name), 0777)
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
			w, err := os.Create(path.Join(dst, hdr.Name))
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
		key := aws.String("assets.tar.xz")

		input := &s3.GetObjectInput{
			Bucket: bucket,
			Key:    key,
		}

		psClient := s3.NewPresignClient(client)

		resp, err := GetPresignedURL(context.TODO(), psClient, input)
		if err != nil {
			log.Fatal(err)
		}

		//fmt.Println(resp.URL)

		httpresp, err := http.Get(resp.URL)
		if err != nil {
			log.Fatal(err)
		}
		defer httpresp.Body.Close()

		out, err := os.Create("./assets.tar.xz")
		if err != nil {
			log.Fatal(err)
		}
		defer out.Close()

		_, err = io.Copy(out, httpresp.Body)
		if err != nil {
			log.Fatal(err)
		}

		extract_xz("./assets.tar.xz", "../../")

		os.Remove("./assets.tar.xz")
	},
}
