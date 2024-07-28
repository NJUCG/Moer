for file in $(ls $1);
do
target/bin/Moer "$1/${file}/"
done
