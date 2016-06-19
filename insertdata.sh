query="insert into node_data values(\"${1}\",${2},now())"
echo $query
mysql -uroot -pinameJay gcsj -e"$query"


