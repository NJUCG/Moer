for file in $(ls $1);
do
target/bin/MOER "$1/${file}/"
done