find "$1" -maxdepth 1 -type d 2> /dev/null | sed '1d' | while read x; do #ищет в папке с переданным названием каталоги (type d)
#глубина поиска - только эта папка
#удаляет первую строку из результата (это сама папка)
#цикл по найденным каталогам
    DIGIT=$(find "$x" -type d 2> /dev/null| sed '1d' | wc -l) 
        if [ $DIGIT -ne 0 ]; then
            t=$(grep -o "/" <<< $x |wc -l)
            let t=t+1
            echo "$x" | cut -d '/' -f $t- 
        fi
done