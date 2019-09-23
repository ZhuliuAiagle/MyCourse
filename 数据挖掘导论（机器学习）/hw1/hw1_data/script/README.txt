The python TA use is version 2.7.

First we use

    python get_all_word_map.py

to generate the all_word_map.pkl and all_word_map.txt. The relationship between word and word index is stored in them. This step filters out marks and some frequent words.

Then we use

    python mkdistribution.py train/ham ham_train.csv
    python mkdistribution.py train/spam spam_train.csv

to generate ham_train.csv and spam_train.csv files which contains the ditribution of all the words in train with respect to ham/spam.

At last we use

    python mkdistribution_each_txt.py test/ham/ ham_test.txt
    python mkdistribution_each_txt.py test/spam/ spam_test.txt

to generate ham_test and spam_test which contains the ditribution of all the words in test with respect to every email. Since it is a huge sparse matrix, the format of the output file is different. For every line is
    
    row index(email id)    column index(word id)   non zero number of occurrences
