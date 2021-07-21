package main

import (
	"context"
	"fmt"
	"io"
	"log"
	"os"
	"path"
	"path/filepath"
	"strings"

	"archive/zip"

	"github.com/aws/aws-sdk-go-v2/aws"
	"github.com/aws/aws-sdk-go-v2/config"
	"github.com/aws/aws-sdk-go-v2/service/s3"
	"github.com/spf13/cobra"
)

func compressDir(dn string) {
	dirpath := filepath.ToSlash(path.Clean(dn))

	fp, err := os.Create("./assets.zip")
	if err != nil {
		log.Fatal(err)
	}
	defer fp.Close()

	w := zip.NewWriter(fp)
	defer w.Close()

	walker := func(pth string, info os.FileInfo, err error) error {
		pth = filepath.ToSlash(pth)
		if err != nil {
			log.Fatal(err)
		}

		if info.IsDir() {
			return nil
		}

		fq, err := os.Open(pth)
		if err != nil {
			return err
		}
		defer fq.Close()

		fixedpath := path.Join(strings.TrimPrefix(path.Dir(pth), dirpath+"/"), path.Base(pth))
		fmt.Printf("\t%s\n", fixedpath)
		fr, err := w.Create(fixedpath)
		if err != nil {
			return err
		}

		_, err = io.Copy(fr, fq)
		if err != nil {
			return err
		}

		return nil
	}

	err = filepath.Walk(dirpath, walker)
	if err != nil {
		panic(err)
	}
}

// S3PutObjectAPI defines the interface for the PutObject function.
// We use this interface to test the function using a mocked service.
type S3PutObjectAPI interface {
	PutObject(ctx context.Context,
		params *s3.PutObjectInput,
		optFns ...func(*s3.Options)) (*s3.PutObjectOutput, error)
}

// PutFile uploads a file to an Amazon Simple Storage Service (Amazon S3) bucket
// Inputs:
//     c is the context of the method call, which includes the AWS Region
//     api is the interface that defines the method call
//     input defines the input arguments to the service call.
// Output:
//     If success, a PutObjectOutput object containing the result of the service call and nil
//     Otherwise, nil and an error from the call to PutObject
func PutFile(c context.Context, api S3PutObjectAPI, input *s3.PutObjectInput) (*s3.PutObjectOutput, error) {
	return api.PutObject(c, input)
}

var pushCmd = &cobra.Command{
	Use:   "push",
	Short: "get version",
	Long:  "version",
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println("compressing...")
		compressDir("../../assets")

		fmt.Println("pushing...")
		bucket := "mode7-assets"
		filename := "assets.zip"

		cfg, err := config.LoadDefaultConfig(context.TODO())
		if err != nil {
			os.Remove("./assets.zip")
			log.Fatal(err)
		}

		client := s3.NewFromConfig(cfg)

		file, err := os.Open(filename)
		if err != nil {
			os.Remove("./assets.zip")
			log.Fatal(err)
		}

		defer file.Close()

		input := &s3.PutObjectInput{
			Bucket: aws.String(bucket),
			Key:    aws.String(filename),
			Body:   file,
		}

		_, err = PutFile(context.TODO(), client, input)
		if err != nil {
			os.Remove("./assets.zip")
			log.Fatal(err)
		}

		os.Remove("./assets.zip")
	},
}
