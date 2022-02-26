./myprog ../../ >> result_c.txt
./my_script.sh ../../ >> result_bash.txt
diff -s result_c.txt result_bash.txt
rm result_c.txt result_bash.txt


./myprog ../ >> result_c.txt
./my_script.sh ../ >> result_bash.txt
diff -s result_c.txt result_bash.txt
rm result_c.txt result_bash.txt


./myprog R >> result_c.txt
./my_script.sh R >> result_bash.txt
diff -s result_c.txt result_bash.txt
rm result_c.txt result_bash.txt


./myprog . >> result_c.txt
./my_script.sh . >> result_bash.txt
diff -s result_c.txt result_bash.txt
rm result_c.txt result_bash.txt