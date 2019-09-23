#python2.7
import sys, os
import cPickle
if len(sys.argv) != 3:
    print 'usage: ' + sys.argv[0] + ' fold_contains_txts output_file'
    sys.exit(0)
files = map(lambda x: os.path.join(sys.argv[1], x), os.listdir(sys.argv[1]))
all_word_map = cPickle.load(open('all_word_map.pkl'))
histogram = [0] * len(all_word_map)
for f in files:
    words = filter(lambda x: x.isalpha() and x in all_word_map, open(f).read().lower().split())
    words_idx = map(lambda x: all_word_map[x], words)
    for wi in words_idx:
        histogram[wi] += 1
output_file = open(sys.argv[2], 'w')
head = True
for h in histogram:
    if head:
        head = False
    else:
        output_file.write(',')
    output_file.write(str(h))
output_file.close()
