package main

import (
	"bufio"
	"fmt"
	"io/ioutil"
	"log"
	"math"
	"os"
	"path/filepath"
	"strings"
	"time"
)

type CommentStyle int

const (
	COMMENT_SLASH CommentStyle = iota
	COMMENT_HASH
	COMMENT_UNKNOWN
)

var blockStartLookup = [...]string{"/**\n", ""}
var blockFillLookup = [...]string{" * ", "# "}
var blockEndLookup = [...]string{"**/\n", ""}

var symbolTable = map[string]string{}

const SIMILARITY_THRESHOLD = 870000

func PrintHelp() {
	fmt.Println("usage: ./licensetool <source file>")
}

func LoadLicenseNotice() string {
	return LoadFileIntoString("./notice.txt")
}

func LoadFileIntoString(fn string) string {
	buf, err := ioutil.ReadFile(fn)
	if err != nil {
		log.Fatalf("error: %v\n", err)
	}

	return string(buf)
}

func DetectCommentStyle(fn string) CommentStyle {
	out := COMMENT_UNKNOWN

	switch filepath.Ext(fn) {
	case ".c", ".h", ".cpp", ".hpp", ".cc", ".hh", ".inl":
		out = COMMENT_SLASH

	case ".py", ".txt", ".sh":
		out = COMMENT_HASH
	}

	return out
}

func GetCommentStyle(style CommentStyle) (string, string, string) {
	return blockStartLookup[style],
		blockFillLookup[style],
		blockEndLookup[style]
}

func DefineSymbol(symbol string, value string) {
	symbolTable[symbol] = value
}

func LookupSymbol(symbol string) string {
	out := symbolTable[symbol]
	if len(out) < 1 {
		out = "!ERROR_NOTFOUND( " + strings.ReplaceAll(symbol, "%", "!") + " )!"
	}

	return out
}

func FillDefaultValues() {
	if len(symbolTable["YEAR"]) < 1 {
		symbolTable["YEAR"] = fmt.Sprint(time.Now().Year())
	}
}

func CommentString(data string, style CommentStyle) string {
	formatted := ""

	blockStart, blockFill, blockEnd := GetCommentStyle(style)

	formatted += blockStart
	scanner := bufio.NewScanner(strings.NewReader(data))
	for scanner.Scan() {
		content := scanner.Text()
		for idx := strings.Index(content, "%"); idx != -1; idx = strings.Index(content, "%") {
			before := content[:idx]

			symbol := content[idx+1:]
			idy := strings.Index(symbol, "%")
			symbol = symbol[:idy]

			after := content[idx+idy+2:]
			content = before + LookupSymbol(symbol) + after
		}
		formatted += blockFill + content + "\n"
	}
	formatted += blockEnd + "\n"

	return formatted
}

func CheckSimilarity(a []byte, b []byte) int {
	la := 0
	lb := 0
	dot := 0
	min := math.Min(float64(len(a)), float64(len(b)))
	max := math.Max(float64(len(a)), float64(len(b)))
	if min/max < 0.6 {
		return 0
	}

	iter := int(min)
	for i := 0; i < iter; i++ {
		la += int(a[i]) * int(a[i])
		lb += int(b[i]) * int(b[i])
		dot += int(a[i]) * int(b[i])
	}
	out := int(math.Round(1000000.0 * (float64(dot) / (math.Sqrt(float64(la)) * math.Sqrt(float64(lb))))))
	return out
}

func InjectNotice(fn string, notice string) {
	src := LoadFileIntoString(fn)

	fp, err := os.Create(fn)
	if err != nil {
		log.Fatal(err)
	}
	defer fp.Close()

	existingLen := int(math.Min(float64(len(notice)), float64(len(src))))
	existingNotice := src[:existingLen]
	if CheckSimilarity([]byte(notice), []byte(existingNotice)) > SIMILARITY_THRESHOLD {
		src = src[strings.Index(src, "\n\n")+2:]
	}

	fp.Write([]byte(notice))
	fp.Write([]byte(src))
}

func main() {
	if len(os.Args) < 2 {
		PrintHelp()
		return
	}

	if len(os.Args) > 2 {
		for i := 2; i < len(os.Args); i++ {
			arg := os.Args[i]
			if strings.HasPrefix(arg, "-D") {
				arg := arg[2:]
				slice := strings.Split(arg, "=")
				DefineSymbol(slice[0], slice[1])
			} else {
				// TODO other flags
			}
		}
	}

	FillDefaultValues()

	notice := LoadLicenseNotice()
	if len(notice) < 1 {
		log.Fatalln("license notice cannot be empty")
	}

	fn := os.Args[1]
	fi, err := os.Stat(fn)
	if err != nil {
		log.Fatal(err)
	}

	switch mode := fi.Mode(); {
	case mode.IsDir():
		err := filepath.Walk(fn, func(path string, info os.FileInfo, rerr error) error {
			if info.Mode().IsRegular() {
				if strings.Contains(path, "3rdparty") || strings.Contains(path, "build") {
					return rerr
				}
                cwd, err := os.Executable()
                if err != nil {
                    log.Fatal(err)
                }
                if strings.Contains(path, filepath.Base(filepath.Dir(cwd))) {
                    return rerr
                }
				style := DetectCommentStyle(path)
				if style == COMMENT_UNKNOWN {
					return rerr
				}
				InjectNotice(path, CommentString(notice, style))
			}
			return rerr
		})

		if err != nil {
			log.Fatal(err)
		}

	case mode.IsRegular():
		style := DetectCommentStyle(fn)
		if style == COMMENT_UNKNOWN {
			fmt.Println("unknown file type")
			return
		}
		InjectNotice(fn, CommentString(notice, style))
	}
}
