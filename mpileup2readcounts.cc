/*  mpileup2readcounts.cc -- Get base counts from mpileup output
    
    Modified by Kent Riemondy

    Copyright (c) 2016,2017 Avinash Ramu
    Author: Avinash Ramu <aramu@genome.wustl.edu>
    
    Copyright (c) 2017,2018 Kent Riemondy

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.  */


#include <iostream>
#include <stdexcept>
#include <sstream>
#include <map>
#include <cctype>
#include <algorithm>
#include <string>

using namespace std;

void usage() {
    std::cerr << "samtools mpileup -f ref.fa -l regions.bed"
              << " alignments.bam | mpileupToReadCounts " << endl
              << "or" << endl
              << "samtools mpileup -f ref.fa -l regions.bed"
              << " alignments.bam | mpileupToReadCounts -d 5 " << endl
              << "to restrict output to only regions with at least 5 reads"
              << endl;
}

//Convert a string to an int
inline int str_to_num(string num) {
    stringstream ss;
    int num_uint;
    ss << num;
    ss >> num_uint;
    return num_uint;
}

map<string,string> bp_map() {
    map<string,string> bps ;    
    bps["T"] = "A" ; 
    bps["A"] = "T" ;
    bps["G"] = "C" ;
    bps["C"] = "G" ;
    bps["N"] = "N" ;
    return bps ;
}

//DS to hold the pertinent information
class mpileup_line {
    public:
        string chr;
        int pos;
        string ref_base;
        int depth;
        string bases;
        string qual;
        
        //Counts for different bases
        int refcountneg, refcountpos;
        int pos_depth, neg_depth ; 
        int acount_pos, ccount_pos, gcount_pos, tcount_pos, ncount_pos;
        int acount_neg, ccount_neg, gcount_neg, tcount_neg, ncount_neg;
        int pos_delcount, neg_delcount, pos_inscount, neg_inscount ;
        int pos_mmcount, neg_mmcount ;

        map<string,string> bps ; 

        mpileup_line() {
            chr = ref_base = bases = qual = "NA";
            depth = pos = 0;
            pos_depth = neg_depth = 0;
            acount_pos = ccount_pos = gcount_pos = tcount_pos = ncount_pos = 0;
            acount_neg = ccount_neg = gcount_neg = tcount_neg = ncount_neg = 0;
            refcountpos = refcountneg = 0 ; 
            pos_inscount = neg_inscount = 0;
            pos_delcount = neg_delcount = 0;
            pos_mmcount = neg_mmcount = 0;
            bps = bp_map() ; 
        }
        
        void set_depth() {
            pos_depth = acount_pos + ccount_pos + gcount_pos + tcount_pos +
                        ncount_pos + pos_inscount + pos_delcount ; 
            neg_depth = acount_neg + ccount_neg + gcount_neg + tcount_neg +
                        ncount_neg + neg_inscount + neg_delcount ;
        }
        
        //Set the appropriate count for ref nucleotide
        void set_ref_nuc_count() {
            switch(ref_base[0]) {
                case 'A':
                    acount_pos = refcountpos;
                    tcount_neg = refcountneg;
                    pos_mmcount = ccount_pos + gcount_pos + tcount_pos;
                    neg_mmcount = ccount_neg + gcount_neg + acount_neg;
                    break;
                case 'C':
                    ccount_pos = refcountpos;
                    gcount_neg = refcountneg;
                    pos_mmcount = acount_pos + gcount_pos + tcount_pos;
                    neg_mmcount = acount_neg + ccount_neg + tcount_neg;
                    break;
                case 'G':
                    gcount_pos = refcountpos;
                    ccount_neg = refcountneg;
                    pos_mmcount = ccount_pos + acount_pos + tcount_pos;
                    neg_mmcount = tcount_neg + gcount_neg + acount_neg; 
                    break;
                case 'T':
                    tcount_pos = refcountpos;
                    acount_neg = refcountneg;
                    pos_mmcount = ccount_pos + gcount_pos + acount_pos;
                    neg_mmcount = ccount_neg + gcount_neg + tcount_neg; 
                    break;
                //count all non-bases as mismatch for N
                case 'N':
                    ncount_pos = refcountpos;
                    ncount_neg = refcountneg;
                    pos_mmcount = tcount_pos + ccount_pos + gcount_pos + acount_pos;
                    neg_mmcount = acount_neg + ccount_neg + gcount_neg + tcount_neg; 
                    break;
                //Deal with -,R,Y,K,M,S,W etc
                default:
                    break;
            }
        }

        static void print_header(ostream& out = cout) {
            out << "chr" << "\t"
                << "pos" << "\t"
                << "strand" << "\t"
                << "depth" << "\t"
                << "ref_base" << "\t"
                << "refcount" << "\t"
                << "acount" << "\t"
                << "ccount" << "\t"
                << "gcount" << "\t"
                << "tcount" << "\t"
                << "ncount" << "\t"
                << "mmcount" << "\t"
                << "delcount" << "\t"
                << "inscount"
                << endl;
        }
        void print_pos(ostream& out = cout) {
            out << chr << "\t"
                << pos << "\t"
                << "+" << "\t"
                << pos_depth << "\t"
                << ref_base << "\t"
                << refcountpos << "\t"
                << acount_pos << "\t"
                << ccount_pos << "\t"
                << gcount_pos << "\t"
                << tcount_pos << "\t"
                << ncount_pos << "\t"
                << pos_mmcount << "\t"
                << pos_delcount << "\t"
                << pos_inscount
                << endl;
        }
        void print_neg(ostream& out = cout) {
            out << chr << "\t"
                << pos << "\t"
                << "-" << "\t"
                << neg_depth << "\t"
                << bps[ref_base] << "\t"
                << refcountneg << "\t"
                << acount_neg << "\t"
                << ccount_neg << "\t"
                << gcount_neg << "\t"
                << tcount_neg << "\t"
                << ncount_neg << "\t"
                << neg_mmcount << "\t"
                << neg_delcount << "\t"
                << neg_inscount
                << endl;
        }
};

//Parse the pileup string, keep indel counts to pass to next line
void parse_bases_to_readcounts(mpileup_line& ml1, 
                               int& pos_previous_dels,
                               int& neg_previous_dels,
                               int& pos_previous_ins,
                               int& neg_previous_ins) {
    
    ml1.neg_delcount = neg_previous_dels ;
    ml1.pos_delcount = pos_previous_dels ;
    ml1.neg_inscount = neg_previous_ins ;
    ml1.pos_inscount = pos_previous_ins ;

    pos_previous_dels = neg_previous_dels = pos_previous_ins = neg_previous_ins = 0;

    for(std::string::size_type i = 0; i < ml1.bases.length(); i++) {
        char base = ml1.bases[i];
        string indelsize_string;
        int indelsize_int = 0;
        switch(base) {
            //Match to reference
            case '.':
                ml1.refcountpos += 1;
                break;
            case ',':
                ml1.refcountneg += 1;
                break;
            
            case 'a':
                ml1.tcount_neg += 1;
                break;
            case 'A':
                ml1.acount_pos += 1;
                break;

            case 'c':
                ml1.gcount_neg += 1;
                break;
            case 'C':
                ml1.ccount_pos += 1;
                break;

            case 'g':
                ml1.ccount_neg += 1;
                break;
            case 'G':
                ml1.gcount_pos += 1;
                break;

            case 't':
                ml1.acount_neg += 1;
                break;
            case 'T':
                ml1.tcount_pos += 1;
                break;

            case 'n':
                ml1.ncount_neg += 1;
                break;
            case 'N':
                ml1.ncount_pos += 1;
                break;

            //Reference skips
            case '<':
            case '>':
                break;
            //This base is deleted
            case '*':
                break;
            
            //Insertion 
            case '+': 
                i++; // advance past + or -
                while(ml1.bases[i] >= 48 && // A char type of 0 is typecast to 48 as int
                      ml1.bases[i] <= 57) { // a char type of 9 is typecase to 57 as int
                    indelsize_string = indelsize_string + ml1.bases[i];
                    i = i+1;
                }

                // check for upper or lower (upper = + strand)
                if (isupper(ml1.bases[i])){
                    pos_previous_ins += 1 ;
                } else {
                    neg_previous_ins += 1 ;
                }

                indelsize_int = str_to_num(indelsize_string);
                i += indelsize_int - 1;
                break;

            //Deletion
            case '-':
                i++; // advance past + or -
                while(ml1.bases[i] >= 48 && // A char type of 0 is typecast to 48 as int
                      ml1.bases[i] <= 57) { // a char type of 9 is typecase to 57 as int
                    indelsize_string = indelsize_string + ml1.bases[i];
                    i = i+1;
                }

                // check for upper or lower (upper = + strand)
                if (isupper(ml1.bases[i])){
                    pos_previous_dels += 1 ;
                } else {
                    neg_previous_dels += 1 ;
                }

                indelsize_int = str_to_num(indelsize_string);
                i += indelsize_int - 1;
                break;
            //End of read segment
            case '$':
                break;
            //Beginning of read segment
            case '^':
                i = i + 1;//Skip quality
                break;
            default:
                throw runtime_error("Unknown ref base: " + to_string(base));
        }
    }

    ml1.set_ref_nuc_count();
    ml1.set_depth();
}

// Parse the pileup string, keep indel counts to pass to next line
// invert the strandedness, useful when strand-specific rev-comp are analyzed
void parse_bases_to_readcounts_reversed(mpileup_line& ml1, 
                               int& pos_previous_dels,
                               int& neg_previous_dels,
                               int& pos_previous_ins,
                               int& neg_previous_ins) {
  
  ml1.neg_delcount = neg_previous_dels ;
  ml1.pos_delcount = pos_previous_dels ;
  ml1.neg_inscount = neg_previous_ins ;
  ml1.pos_inscount = pos_previous_ins ;
  
  pos_previous_dels = neg_previous_dels = pos_previous_ins = neg_previous_ins = 0;
  
  for(std::string::size_type i = 0; i < ml1.bases.length(); i++) {
    char base = ml1.bases[i];
    string indelsize_string;
    int indelsize_int = 0;
    switch(base) {
    //Match to reference
    case '.':
      ml1.refcountneg += 1;
      break;
    case ',':
      ml1.refcountpos += 1;
      break;
    
    case 'a':
      ml1.acount_pos += 1;
      break;
    case 'A':
      ml1.tcount_neg += 1;
      break;
      
    case 'c':
      ml1.gcount_pos += 1;
      break;
    case 'C':
      ml1.gcount_neg += 1;
      break;
      
    case 'g':
      ml1.ccount_pos += 1;
      break;
    case 'G':
      ml1.ccount_neg += 1;
      break;
      
    case 't':
      ml1.acount_pos += 1;
      break;
    case 'T':
      ml1.acount_neg += 1;
      break;
      
    case 'n':
      ml1.ncount_pos += 1;
      break;
    case 'N':
      ml1.ncount_neg += 1;
      break;
      
      //Reference skips
    case '<':
    case '>':
      break;
      //This base is deleted
    case '*':
      break;
      
      //Insertion 
    case '+': 
      i++; // advance past + or -
      while(ml1.bases[i] >= 48 && // A char type of 0 is typecast to 48 as int
            ml1.bases[i] <= 57) { // a char type of 9 is typecase to 57 as int
        indelsize_string = indelsize_string + ml1.bases[i];
        i = i+1;
      }
      
      // check for upper or lower (upper = + strand)
      if (isupper(ml1.bases[i])){
        neg_previous_ins += 1 ;
      } else {
        pos_previous_ins += 1 ;
      }
      
      indelsize_int = str_to_num(indelsize_string);
      i += indelsize_int - 1;
      break;
      
      //Deletion
    case '-':
      i++; // advance past + or -
      while(ml1.bases[i] >= 48 && // A char type of 0 is typecast to 48 as int
            ml1.bases[i] <= 57) { // a char type of 9 is typecase to 57 as int
        indelsize_string = indelsize_string + ml1.bases[i];
        i = i+1;
      }
      
      // check for upper or lower (upper = + strand)
      // invert for reporting
      if (isupper(ml1.bases[i])){
        neg_previous_dels += 1 ;
      } else {
        pos_previous_dels += 1 ;
      }
      
      indelsize_int = str_to_num(indelsize_string);
      i += indelsize_int - 1;
      break;
      //End of read segment
    case '$':
      break;
      //Beginning of read segment
    case '^':
      i = i + 1;//Skip quality
      break;
    default:
      throw runtime_error("Unknown ref base: " + to_string(base));
    }
  }
  
  ml1.set_ref_nuc_count();
  ml1.set_depth();
}

//Split the line into the required fields and parse
void process_mpileup_line(std::string line, 
                         int min_depth,
                         int& pos_previous_dels,
                         int& neg_previous_dels,
                         int& pos_previous_ins,
                         int& neg_previous_ins,
                         bool reversed) {
    stringstream ss(line);
    mpileup_line ml1;
    string pos, depth;

    getline(ss, ml1.chr, '\t');
    
    getline(ss, pos, '\t');
    ml1.pos = str_to_num(pos);
    
    string base;
    getline(ss, base, '\t');
    transform(base.begin(), base.end(), base.begin(), ::toupper) ;
    ml1.ref_base = base;
    
    getline(ss, depth, '\t');
    ml1.depth = str_to_num(depth);
    
    getline(ss, ml1.bases, '\t');
    getline(ss, ml1.qual, '\t');
    
    if (!reversed){
      parse_bases_to_readcounts(ml1, 
                                pos_previous_dels,
                                neg_previous_dels,
                                pos_previous_ins,
                                neg_previous_ins);
    } else {
      parse_bases_to_readcounts_reversed(ml1, 
                                pos_previous_dels,
                                neg_previous_dels,
                                pos_previous_ins,
                                neg_previous_ins);
    }
   
    if(ml1.pos_depth >= min_depth) ml1.print_pos();
    if(ml1.neg_depth >= min_depth) ml1.print_neg();
    
}

int main(int argc, char* argv[]) {
    
    // only report regions with >= 1 reads by default
    int min_depth = 1 ;
    // don't report as opposite strand by default
    bool reversed = false ;

    if (argc > 1) {
        std::string arg = argv[1];
        if ((arg == "-h") || (arg == "--help")) {
            usage();
            return 1;
        } else if (arg == "-d") {
            if (argc > 2) { 
                min_depth = stoi(argv[2]); 
            } else {
                cerr << "-d requires one argument" << endl;
                return 1;
            }
        } else if (arg == "-r"){
            reversed = true ;
        } else {
            usage();
            return 1;
        }
        
        if (argc > 3){
            std::string arg = argv[3];
            if (arg == "-r"){
              reversed = true ;
            } else {
              usage();
              return 1;
            }
        }
    } 
    
    
    string line;
    mpileup_line::print_header();
    getline(cin, line);
    
    int pos_previous_dels, neg_previous_dels, pos_previous_ins, neg_previous_ins;
    pos_previous_dels = neg_previous_dels = pos_previous_ins = neg_previous_ins = 0;
    
    while(cin) {
        try {
            // store indel count and add to next line
            process_mpileup_line(line, 
                                 min_depth,
                                 pos_previous_dels,
                                 neg_previous_dels,
                                 pos_previous_ins,
                                 neg_previous_ins,
                                 reversed);
        } catch(const std::runtime_error& e) {
            cerr << e.what() << endl;
            cerr << "\nError parsing line " << line;
            break;
        }
        getline(cin, line);
    }
}
