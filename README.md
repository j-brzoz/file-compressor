# File compressor
## About
Project using 'C' language for JiMP2 class in 2023L.
## Description
Program which allows users to compress and decompress non-empty files. Features include:
 - 3 compression rates
 - password protection
 - verbose mode
## Usage
In order to use compressor, you need to type *make compressor* in the terminal while being in project's directory.
### To compress:
Usage: compressor -x input-file -o output-file -compress-rate [-c password -v -h] <br>
    -x input-file <br>
    -o output-file <br>
    -compress-rate &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Available compression rates: -1 -2 -3 <br>
    -c password <br>
    -v &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; verbose <br>
    -h &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; print usage
### To decompress:
Usage: compressor -z input-file -o output-file -compress-rate [-c [password] -v -h] <br>
-z input-file <br>
-o output-file <br>
-compress-rate  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Available compression rates: -1 -2 -3 <br>
-c password <br>
-v  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; verbose <br>
-h &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; print usage <br>
## Available tests  
- easy.txt 11B - 5 different ASCII characters
- file.txt 268B – many  different ASCII characters
- one-letter.txt 1B – 1 ASCII character 
- one-type.txt 8B – 1 ASCII character
- alphabet.txt 1MB – capital letters
- gen.txt 128KB – every possible 2-character ASCII combination
- lotr.txt 3,15MB - *“Lord of the Rings”*
- pan-tadeusz.txt 482KB - *“Pan Tadeusz”* 
- test-jpg.jpg 826KB – .jpg  file
- test-pdf.pdf 4,22MB – .pdf file
- test-png.png 2,26MB – .png file
## How to test
If you want to test:
- 8 bit compression on small text tests – *make test-8-small* 
- 8 bit compression on bigger .txt, .pdf, .jpg, .png tests – *make test-8-big* 
- 8 bit compression on all tests – *make test-8-all*
- 12 bit compression on small text tests – *make test-12-small* 
- 12 bit compression on bigger .txt, .pdf, .jpg, .png tests – *make test-12-big* 
- 12 bit compression on all tests – *make test-12-all* 
- 16 bit compression on small text tests – make *test-16-small* 
- 16 bit compression on bigger .txt, .pdf, .jpg, .png tests – *make test-16-big* 
- 16 bit compression on all tests – *make test-16-all*
- 8, 12 and 16 bit compression on small text tests – *make test-small* 
- 8, 12 and 16 bit compression on bigger .txt, .pdf, .jpg, .png tests – *make test-big* 
- 8, 12 and 16 bit compression on all tests – *make test-all* 
- password protection for 8, 12 and 16 bit compression, verbose mode and help message - *make test-features* 
## Authors
- Jan Brzozowski
- Jan Gębal