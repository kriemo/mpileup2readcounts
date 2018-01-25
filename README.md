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
```

## Example output
```
chr     pos     strand  depth   ref_base        refcount        acount  ccount  gcount  tcount  ncount  mmcount delcount        inscount
chr1    10021   -       359     T       355     0       0       4       355     0       4       0       0
chr1    10022   +       179     C       179     0       179     0       0       0       0       0       0
chr1    10022   -       413     G       408     5       0       408     0       0       5       0       0
chr1    10023   +       188     C       188     0       188     0       0       0       0       0       0
chr1    10023   -       418     G       418     0       0       418     0       0       0       0       0
chr1    10024   +       194     C       193     1       193     0       0       0       1       0       0
chr1    10024   -       421     G       420     1       0       420     0       0       1       0       0
chr1    10025   +       195     T       194     1       0       0       194     0       1       0       0
chr1    10025   -       424     A       424     424     0       0       0       0       0       0       0
chr1    10026   +       195     A       195     195     0       0       0       0       0       0       0
chr1    10026   -       403     T       403     0       0       0       403     0       0       0       0
chr1    10027   +       195     A       194     194     0       1       0       0       1       0       0
chr1    10027   -       403     T       401     0       0       2       401     0       2       0       0
```
