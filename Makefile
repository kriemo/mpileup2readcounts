CC = g++
CFLAGS = -std=c++0x -Wall -O2
#LIBS = -lz
MKDIR_P := mkdir -p
OUT_DIR := bin

all: $(OUT_DIR)/mpileupToReadCounts

$(OUT_DIR)/mpileupToReadCounts: mpileup2readcounts.cc
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)
clean:
	rm -rf $(OUT_DIR)


