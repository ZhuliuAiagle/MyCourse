#python2.7
from os import walk, path
import cPickle
all_word = set()
high_freq_word = set(['a', 'the', 'to'])
all_file = []
for (dirpath, dirnames, filenames) in walk('./'):
    for f in filenames:
        if f.endswith('.txt') and dirpath.startswith('./train'):
            all_file.append(path.join(dirpath, f))

for f in all_file:
    word_set = set(filter(lambda x: x.isalpha() and len(x) > 1, open(f).read().lower().split()))
    all_word = all_word.union(word_set)

all_word = all_word.difference(high_freq_word)
all_word_map = dict(zip(list(all_word), range(len(all_word))))
output_file = open('all_word_map.txt', 'w')
for item in zip(list(all_word), range(len(all_word))):
    output_file.write(item[0] + '\t' + str(item[1]+1) + '\n')
output_file.close()
cPickle.dump(all_word_map, open('all_word_map.pkl', 'w'))
