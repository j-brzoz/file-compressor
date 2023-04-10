compressor: 
	@$(CC) src/main.c src/compRate/eight.c src/compRate/sixteen.c src/compRate/twelve.c src/queue/node.c src/queue/queue.c src/tools/convert.c src/tools/fileHeader.c src/tools/dictionary.c -o compressor -lm

compress-8-small: compressor
	@./compressor -x tests/small/easy.txt -o easy8.huff -1
	@./compressor -z easy8.huff -o easy8.txt
	@./compressor -x tests/small/file.txt -o file8.huff -1
	@./compressor -z file8.huff -o file8.txt
	@./compressor -x tests/small/one-letter.txt -o one-letter8.huff -1
	@./compressor -z one-letter8.huff -o one-letter8.txt
	@./compressor -x tests/small/one-type.txt -o one-type8.huff -1
	@./compressor -z one-type8.huff -o one-type8.txt

compress-8-big: compressor
	@./compressor -x tests/big/alphabet.txt -o alphabet8.huff -1
	@./compressor -z alphabet8.huff -o alphabet8.txt
	@./compressor -x tests/big/gen.txt -o gen8.huff -1
	@./compressor -z gen8.huff -o gen8.txt
	@./compressor -x tests/big/lotr.txt -o lotr8.huff -1
	@./compressor -z lotr8.huff -o lotr8.txt
	@./compressor -x tests/big/pan-tadeusz.txt -o pan-tadeusz8.huff -1
	@./compressor -z pan-tadeusz8.huff -o pan-tadeusz8.txt
	@./compressor -x tests/big/test-jpg.jpg -o test-jpg8.huff -1
	@./compressor -z test-jpg8.huff -o test-jpg8.jpg
	@./compressor -x tests/big/test-pdf.pdf -o test-pdf8.huff -1
	@./compressor -z test-pdf8.huff -o test-pdf8.pdf
	@./compressor -x tests/big/test-png.png -o test-png8.huff -1
	@./compressor -z test-png8.huff -o test-png8.png

test-8-small: compress-8-small
	@cmp -s tests/small/easy.txt easy8.txt && echo "8 - SAME - easy.txt" || echo "8 - DIFF - easy.txt"
	@cmp -s tests/small/file.txt file8.txt && echo "8 - SAME - file.txt" || echo "8 - DIFF - file.txt"
	@cmp -s tests/small/one-letter.txt one-letter8.txt && echo "8 - SAME - one-letter.txt" || echo "8 - DIFF - one-letter.txt"
	@cmp -s tests/small/one-type.txt one-type8.txt && echo "8 - SAME - one-type.txt" || echo "8 - DIFF - one-type.txt"
	@rm *.huff
	@rm *.txt

test-8-big: compress-8-big
	@cmp -s tests/big/alphabet.txt alphabet8.txt && echo "8 - SAME - alphabet.txt" || echo "8 - DIFF - alphabet.txt"
	@cmp -s tests/big/gen.txt gen8.txt && echo "8 - SAME - gen.txt" || echo "8 - DIFF - gen.txt"
	@cmp -s tests/big/lotr.txt lotr8.txt && echo "8 - SAME - lotr.txt" || echo "8 - DIFF - lotr.txt"
	@cmp -s tests/big/pan-tadeusz.txt pan-tadeusz8.txt && echo "8 - SAME - pan-tadeusz.txt" || echo "8 - DIFF - pan-tadeusz.txt"
	@cmp -s tests/big/test-jpg.jpg test-jpg8.jpg && echo "8 - SAME - test-jpg.jpg" || echo "8 - DIFF - test-jpg.jpg"
	@cmp -s tests/big/test-pdf.pdf test-pdf8.pdf && echo "8 - SAME - test-pdf.pdf" || echo "8 - DIFF - test-pdf.pdf"
	@cmp -s tests/big/test-png.png test-png8.png && echo "8 - SAME - test-png.png" || echo "8 - DIFF -test-png.png"
	@rm *.huff
	@rm *.txt
	@rm *.jpg
	@rm *.pdf
	@rm *.png

test-8-all: test-8-small test-8-big

compress-12-small: compressor
	@./compressor -x tests/small/easy.txt -o easy12.huff -2
	@./compressor -z easy12.huff -o easy12.txt
	@./compressor -x tests/small/file.txt -o file12.huff -2
	@./compressor -z file12.huff -o file12.txt
	@./compressor -x tests/small/one-letter.txt -o one-letter12.huff -2
	@./compressor -z one-letter12.huff -o one-letter12.txt
	@./compressor -x tests/small/one-type.txt -o one-type12.huff -2
	@./compressor -z one-type12.huff -o one-type12.txt

compress-12-big: compressor
	@./compressor -x tests/big/alphabet.txt -o alphabet12.huff -2
	@./compressor -z alphabet12.huff -o alphabet12.txt
	@./compressor -x tests/big/gen.txt -o gen12.huff -2
	@./compressor -z gen12.huff -o gen12.txt
	@./compressor -x tests/big/lotr.txt -o lotr12.huff -2
	@./compressor -z lotr12.huff -o lotr12.txt
	@./compressor -x tests/big/pan-tadeusz.txt -o pan-tadeusz12.huff -2
	@./compressor -z pan-tadeusz12.huff -o pan-tadeusz12.txt
	@./compressor -x tests/big/test-jpg.jpg -o test-jpg12.huff -2
	@./compressor -z test-jpg12.huff -o test-jpg12.jpg
	@./compressor -x tests/big/test-pdf.pdf -o test-pdf12.huff -2
	@./compressor -z test-pdf12.huff -o test-pdf12.pdf
	@./compressor -x tests/big/test-png.png -o test-png12.huff -2
	@./compressor -z test-png12.huff -o test-png12.png

test-12-small: compress-12-small
	@cmp -s tests/small/easy.txt easy12.txt && echo "12 - SAME - easy.txt" || echo "12 - DIFF - easy.txt"
	@cmp -s tests/small/file.txt file12.txt && echo "12 - SAME - file.txt" || echo "12 - DIFF - file.txt"
	@cmp -s tests/small/one-letter.txt one-letter12.txt && echo "12 - SAME - one-letter.txt" || echo "12 - DIFF - one-letter.txt"
	@cmp -s tests/small/one-type.txt one-type12.txt && echo "12 - SAME - one-type.txt" || echo "12 - DIFF - one-type.txt"
	@rm *.huff
	@rm *.txt

test-12-big: compress-12-big
	@cmp -s tests/big/alphabet.txt alphabet12.txt && echo "12 - SAME - alphabet.txt" || echo "12 - DIFF - alphabet.txt"
	@cmp -s tests/big/gen.txt gen12.txt && echo "12 - SAME - gen.txt" || echo "12 - DIFF - gen.txt"
	@cmp -s tests/big/lotr.txt lotr12.txt && echo "12 - SAME - lotr.txt" || echo "12 - DIFF - lotr.txt"
	@cmp -s tests/big/pan-tadeusz.txt pan-tadeusz12.txt && echo "12 - SAME - pan-tadeusz.txt" || echo "12 - DIFF - pan-tadeusz.txt"
	@cmp -s tests/big/test-jpg.jpg test-jpg12.jpg && echo "12 - SAME - test-jpg.jpg" || echo "12 - DIFF - test-jpg.jpg"
	@cmp -s tests/big/test-pdf.pdf test-pdf12.pdf && echo "12 - SAME - test-pdf.pdf" || echo "12 - DIFF - test-pdf.pdf"
	@cmp -s tests/big/test-png.png test-png12.png && echo "12 - SAME - test-png.png" || echo "12 - DIFF -test-png.png"
	@rm *.huff
	@rm *.txt
	@rm *.jpg
	@rm *.pdf
	@rm *.png

test-12-all: test-12-small test-12-big

compress-16-small: compressor
	@./compressor -x tests/small/easy.txt -o easy16.huff -3
	@./compressor -z easy16.huff -o easy16.txt
	@./compressor -x tests/small/file.txt -o file16.huff -3
	@./compressor -z file16.huff -o file16.txt
	@./compressor -x tests/small/one-letter.txt -o one-letter16.huff -3
	@./compressor -z one-letter16.huff -o one-letter16.txt
	@./compressor -x tests/small/one-type.txt -o one-type16.huff -3
	@./compressor -z one-type16.huff -o one-type16.txt

compress-16-big: compressor
	@./compressor -x tests/big/alphabet.txt -o alphabet16.huff -3
	@./compressor -z alphabet16.huff -o alphabet16.txt
	@./compressor -x tests/big/gen.txt -o gen16.huff -3
	@./compressor -z gen16.huff -o gen16.txt
	@./compressor -x tests/big/lotr.txt -o lotr16.huff -3
	@./compressor -z lotr16.huff -o lotr16.txt
	@./compressor -x tests/big/pan-tadeusz.txt -o pan-tadeusz16.huff -3
	@./compressor -z pan-tadeusz16.huff -o pan-tadeusz16.txt
	@./compressor -x tests/big/test-jpg.jpg -o test-jpg16.huff -3
	@./compressor -z test-jpg16.huff -o test-jpg16.jpg
	@./compressor -x tests/big/test-pdf.pdf -o test-pdf16.huff -3
	@./compressor -z test-pdf16.huff -o test-pdf16.pdf
	@./compressor -x tests/big/test-png.png -o test-png16.huff -3
	@./compressor -z test-png16.huff -o test-png16.png

test-16-small: compress-16-small
	@cmp -s tests/small/easy.txt easy16.txt && echo "16 - SAME - easy.txt" || echo "16 - DIFF - easy.txt"
	@cmp -s tests/small/file.txt file16.txt && echo "16 - SAME - file.txt" || echo "16 - DIFF - file.txt"
	@cmp -s tests/small/one-letter.txt one-letter16.txt && echo "16 - SAME - one-letter.txt" || echo "16 - DIFF - one-letter.txt"
	@cmp -s tests/small/one-type.txt one-type16.txt && echo "16 - SAME - one-type.txt" || echo "16 - DIFF - one-type.txt"
	@rm *.huff
	@rm *.txt

test-16-big: compress-16-big
	@cmp -s tests/big/alphabet.txt alphabet16.txt && echo "16 - SAME - alphabet.txt" || echo "16 - DIFF - alphabet.txt"
	@cmp -s tests/big/gen.txt gen16.txt && echo "16 - SAME - gen.txt" || echo "16 - DIFF - gen.txt"
	@cmp -s tests/big/lotr.txt lotr16.txt && echo "16 - SAME - lotr.txt" || echo "16 - DIFF - lotr.txt"
	@cmp -s tests/big/pan-tadeusz.txt pan-tadeusz16.txt && echo "16 - SAME - pan-tadeusz.txt" || echo "16 - DIFF - pan-tadeusz.txt"
	@cmp -s tests/big/test-jpg.jpg test-jpg16.jpg && echo "16 - SAME - test-jpg.jpg" || echo "16 - DIFF - test-jpg.jpg"
	@cmp -s tests/big/test-pdf.pdf test-pdf16.pdf && echo "16 - SAME - test-pdf.pdf" || echo "16 - DIFF - test-pdf.pdf"
	@cmp -s tests/big/test-png.png test-png16.png && echo "16 - SAME - test-png.png" || echo "16 - DIFF - test-png.png"
	@rm *.huff
	@rm *.txt
	@rm *.jpg
	@rm *.pdf
	@rm *.png

test-16-all: test-16-small test-16-big

test-small: test-8-small test-12-small test-16-small

test-big: test-8-big test-12-big test-16-big

test-all: test-8-all test-12-all test-16-all

test-features: compressor
	@echo "========== Test password protection =========="
	@./compressor -x tests/small/easy.txt -o easy8.huff -1 -c password
	@./compressor -z easy8.huff -o easy8.txt -c password
	@./compressor -x tests/small/easy.txt -o easy12.huff -2 -c password
	@./compressor -z easy12.huff -o easy12.txt -c password
	@./compressor -x tests/small/easy.txt -o easy16.huff -3 -c password
	@./compressor -z easy16.huff -o easy16.txt -c password
	@cmp -s tests/small/easy.txt easy8.txt && echo "8 - SAME - easy.txt with password" || echo "8 - DIFF - easy.txt with password"
	@cmp -s tests/small/easy.txt easy12.txt && echo "12 - SAME - easy.txt with password" || echo "12 - DIFF - easy.txt with password"
	@cmp -s tests/small/easy.txt easy16.txt && echo "16 - SAME - easy.txt with password" || echo "16 - DIFF - easy.txt with password"
	@echo "========== Test verbose mode with compression =========="
	@./compressor -x tests/small/easy.txt -o easy8.huff -1 -v
	@echo "========== Test verbose mode with decompression =========="
	@./compressor -z easy8.huff -o easy8.txt -v
	@echo "========== Test help message =========="
	@./compressor -h
	@rm *.txt
	@rm *.huff

clean-small:
	@rm compressor
	@rm *.txt
	@rm *.huff

clean-big:
	@rm compressor
	@rm *.txt
	@rm *.huff
	@rm *.jpg
	@rm *.pdf
	@rm *.png