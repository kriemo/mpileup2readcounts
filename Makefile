CC = g++
CFLAGS = -ggdb -Wall -O2
#LIBS = -lz
MKDIR_P := mkdir -p
OUT_DIR := bin

all: $(OUT_DIR)/mpileup2readcounts

$(OUT_DIR)/mpileup2readcounts: mpileup2readcounts.cc
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)
clean:
	rm -rf $(OUT_DIR)


