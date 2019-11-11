#!/opt/local/bin/python

"""
# This script fixes the Liberty models so that they can be read
# in by the SNPS open source Liberty parser.
#
# 1.  forces all argumenst in index_* to be strings, even if they
#     have a single value.
#
#     Example:
#
#         index_1(0.00472397);
#     becomes
#         index_1 ("0.00472397");
#
#  2. forces all near-zero values to have 4 significant figures
#
#     Example:
#         values("-1e-09,-0.173291,-0.17644
#     becomes
#         values("-1.0000e-09,-0.173291,-0.17644
"""
import argparse
import os
import re


# match index_1(0.00472397);
INDEX_PATTERN = re.compile(r'(\s*)(index_\d)\(([\d\.]+)\)')

# match values("-1e-09...

ZERO_PATTERN1 = re.compile(r'(\s*)values\("1e\-06(.*)')
ZERO_PATTERN2 = re.compile(r'(\s*)values\("\-1e\-06(.*)')
ZERO_PATTERN3 = re.compile(r'(\s*)values\("\-1e\-09(.*)')

def main():
    """main routine"""

    parser = argparse.ArgumentParser()
    parser.add_argument('--input', required=True)
    parser.add_argument('--output', required=True)

    args = parser.parse_args()

    if not os.path.exists(args.input):
        print "Cannot open input file %s" % args.input

    infile = open(args.input, "r")

    buf = infile.readlines()

    outfile = open(args.output, "w")

    for line in buf:
        match = INDEX_PATTERN.search(line)
        match1 = ZERO_PATTERN1.search(line)
        match2 = ZERO_PATTERN2.search(line)
        match3 = ZERO_PATTERN3.search(line)

        if match:
            spaces = match.group(1)
            prefix = match.group(2)
            value = match.group(3)
            outfile.write("%s%s (\"%s\");\n" % (spaces, prefix, value))
            continue

        if match1:
            spaces = match1.group(1)
            suffix = match1.group(2)
            outfile.write("%svalues(\"1.0000e-06%s" % (spaces, suffix))
            outfile.write("\n")
            continue

        if match2:
            spaces = match2.group(1)
            suffix = match2.group(2)
            outfile.write("%svalues(\"-1.0000e-06%s" % (spaces, suffix))
            outfile.write("\n")
            continue

        if match3:
            spaces = match3.group(1)
            suffix = match3.group(2)
            outfile.write("%svalues(\"-1.0000e-09%s" % (spaces, suffix))
            outfile.write("\n")
            continue

        outfile.write(line)

    exit(0)


if __name__ == "__main__":
    main()
