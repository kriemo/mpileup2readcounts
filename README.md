[![Build Status](https://travis-ci.org/kriemo/mpileup2readcounts.svg?branch=master)](https://travis-ci.org/kriemo/mpileup2readcounts)

## Synopsis

Get the readcounts at a locus in a strand specific manner by piping samtools mpileup output.
This program has been tested on samtools v1.6

## Install

```bash
git clone git@github.com:kriemo/mpileup2readcounts
cd mpileup2readcounts
make
```

## Usage
```bash
samtools mpileup \
  -f ref.fa \
  -l regions.bed \
  alignments.bam \
  | mpileupToReadCounts -
```

## Example output
```
chr     pos     strand  depth   ref_base        refcount        acount  ccount  gcount  tcount  ncount  delcount        inscount
chr1    1393522 +       1       G       1       0       0       1       0       0       0       0
chr1    1393523 +       1       T       1       0       0       0       1       0       0       0
chr1    1393524 +       1       G       1       0       0       1       0       0       0       0
chr1    1393525 +       1       G       1       0       0       1       0       0       0       0
chr1    1393526 +       1       G       1       0       0       1       0       0       0       0
chr1    1393527 +       1       T       1       0       0       0       1       0       0       0
chr1    1393528 +       1       G       1       0       0       1       0       0       0       0
chr1    1393529 +       1       T       1       0       0       0       1       0       0       0
chr1    1393530 +       1       C       1       0       1       0       0       0       0       0
chr1    1393531 +       1       C       1       0       1       0       0       0       0       0
chr1    1393532 +       1       A       1       1       0       0       0       0       0       0
```
