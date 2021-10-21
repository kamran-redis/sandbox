gcc -g -D INTERVALTREE_MALLOC_INCLUDE=\"intervaltree_malloc.h\" -std=c99    -lm -fno-common -W -Wall  intervaltree.c  client.c

src/redis-server --protected-mode no --bind 127.0.0.1  --loadmodule ../module/it/src/intervaltreetype.so

for i in {0..100}; do  src/redis-cli it.add k1 $i $i $i ; done