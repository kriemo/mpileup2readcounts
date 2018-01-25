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
  | mpileupToReadCounts

## only report positions with depth >= 5 reads

samtools mpileup \
  -f ref.fa \
  -l regions.bed \
  alignments.bam \
  | mpileupToReadCounts -d 5

## only report positions with depth >= 5 reads
## report complement positions (useful for strand-specific fr-firststranded libraries)

samtools mpileup \
  -f ref.fa \
  -l regions.bed \
  alignments.bam \
  | mpileupToReadCounts -d 5 -r
```

## Example output
```
chr	pos	strand	depth	ref_base	refcount	acount	ccount	gcount	tcount	ncount	mmcount	delcount	inscount
chr1	10004	-	8	G	8	0	0	8	0	0	0	0	0
chr1	10005	-	10	G	10	0	0	10	0	0	0	0	0
chr1	10006	-	12	G	12	0	0	12	0	0	0	0	0
chr1	10007	-	13	A	13	13	0	0	0	0	0	0	0
chr1	10008	-	11	T	10	1	0	0	10	0	1	0	0
chr1	10009	-	11	T	11	0	0	0	11	0	0	0	0
chr1	10010	-	22	G	22	0	0	22	0	0	0	0	0
chr1	10011	-	23	G	23	0	0	23	0	0	0	0	0
chr1	10012	-	25	G	25	0	0	25	0	0	0	0	0
chr1	10013	-	26	A	26	26	0	0	0	0	0	0	0
chr1	10014	-	22	T	22	0	0	0	22	0	0	0	0
chr1	10015	-	24	T	24	0	0	0	24	0	0	0	0
```

with -r set

```
chr	pos	strand	depth	ref_base	refcount	acount	ccount	gcount	tcount	ncount	mmcount	delcount	inscount
chr1	10004	+	8	C	8	0	8	0	0	0	0	0	0
chr1	10005	+	10	C	10	0	10	0	0	0	0	0	0
chr1	10006	+	12	C	12	0	12	0	0	0	0	0	0
chr1	10007	+	13	T	13	0	0	0	13	0	0	0	0
chr1	10008	+	10	A	10	10	0	0	0	0	0	0	0
chr1	10009	+	11	A	11	11	0	0	0	0	0	0	0
chr1	10010	+	22	C	22	0	22	0	0	0	0	0	0
chr1	10011	+	23	C	23	0	23	0	0	0	0	0	0
chr1	10012	+	25	C	25	0	25	0	0	0	0	0	0
chr1	10013	+	26	T	26	0	0	0	26	0	0	0	0
chr1	10014	+	22	A	22	22	0	0	0	0	0	0	0
chr1	10015	+	24	A	24	24	0	0	0	0	0	0	0
chr1	10016	+	30	C	30	0	30	0	0	0	0	0	0
chr1	10016	-	6	G	6	0	0	6	0	0	0	0	0
chr1	10017	+	29	C	29	0	29	0	0	0	0	0	0
chr1	10017	-	7	G	7	0	0	7	0	0	0	0	0
chr1	10018	+	32	C	32	0	32	0	0	0	0	0	0
```
