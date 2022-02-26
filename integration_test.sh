./myprog ../../ >> result_c.txt
echo  ".brew
.config
.docker
.Trash
.vscode
C3_SimpleBashUtils-0
Desktop
Documents
Downloads
Library
Movies" >> result.txt
diff -s result_c.txt result.txt
rm result_c.txt result.txt



(./myprog g) 2> result_c.txt
echo  "\x1B[31mДиректории с таким названием не существует" >> result.txt
diff -s result_c.txt result.txt
rm result_c.txt result.txt


(./myprog R/f) > result_c.txt
echo  "\x1B[33mВ данном каталоге подходящих каталогов не найдено" >> result.txt
diff -s result_c.txt result.txt
rm result_c.txt result.txt


(./myprog) 2> result_c.txt
echo  "\x1B[31mКаталог не задан" >> result.txt
diff -s result_c.txt result.txt
rm result_c.txt result.txt
