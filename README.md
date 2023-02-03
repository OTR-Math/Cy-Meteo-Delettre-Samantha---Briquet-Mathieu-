# Cy-Meteo Samantha D. & Mathieu B.
Meteo

To use this program, the instructions are listed with the command:
./main.sh --help

The file data.csv ( initially named meteo_filtered_data ) is not complete here, you can download it elsewhere.

There are 9 different filtering options:
    -w (wind) : average wind orientation 'X' and average wind speed 'Y'.
    -h (height) : altitude in descending order.
    -m (moisture) : moisture in descending order.
    -t (temperature) :
        -t1 : minimum,maximum and average temperatures from each station in ascending order.
        -t2 : average temperatures in chronological order.
        -t3 : average temperatures in chronological order and ascending order of ID's station.
    -p (pressure) : 
        -p1 : minimum,maximum and average pressures from each station in ascending order.
        -p2 : average pressures in chronological order.
        -p3 : average pressures in chronological order and ascending order of ID's station.
                        
There are 9 different places : 
    -A : West Indies;
    -F : Mainland France and Corsica;
    -Q : Antarctic;
    -G : Guyana;
    -O : Indian Ocean;
    -S : Saint Pierre and Miquelon.
                    
To verify if your file is compatible with this program :
    -f <file>.
                    
There are 3 different type of sorting :
    --avl : sorting by binary search and balanced tree;
    --abr : sorting by binary search tree.
    
                    
--help  affiche l'aide
                
Exemples:
    - ./main.sh -f $METEO_RESOURCES_PATH/data.csv -t1 -A --avl
    - ./main.sh -Q --tab -f $METEO_RESOURCES_PATH/data.csv -w
