let "check=0"
touch file.txt
if [ "$1" != "" ]; then
    if [ -d "$1" ]; then
    find "$1" -maxdepth 1 -type d | sed '1d' | while read x; do #ищет в папке с переданным названием каталоги (type d)
    #глубина поиска - только эта папка
    #удаляет первую строку из полученного результата (это сама папка)
    #цикл по найденным каталогам
    DIGIT=$(find "$x" -type d | sed '1d' | wc -l)
        if [ $DIGIT -ne 0 ]; then
            t=$(grep -o "/" <<< $x |wc -l);
            let t=t+1
            let "check=check+1"
            echo "$x" | cut -d '/' -f $t-   
        fi
    
    echo $check > file.txt
    done
    read check < file.txt
    #echo $check
    if [ $check -eq 0 ]; then
        echo "В данном каталоге подходящих каталогов не найдено"
    fi
else
    echo "Директории с таким названием не существует" 1>&2
fi
    else 
    echo "Каталог не задан" 1>&2
fi
rm -rf file.txt

