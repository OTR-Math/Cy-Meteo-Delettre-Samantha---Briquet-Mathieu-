#!/bin/bash
#!/usr/local/bin/gnuplot -persist

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#~ CODE C init & functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
readonly VERSION=0.2

readonly SUCCESS=0
readonly ERR_OPTIONS=1
readonly ERR_DATA_IN=2
readonly ERR_FILES_OUT=3
readonly ERR_INTERNAL=4

readonly METEO_RESOURCES_PATH="./resources"
readonly PROG_C_APP="./meteo_parser"
readonly PROG_C_INPUT_FILE="temp2order.csv"
readonly PROG_C_OUTPUT_FILE_BASE="./generated/tempOrdered"
readonly GNUPLOT_DATA_EXT=".gpdat"
readonly GNUPLOT_SCRIPT="./generated/gnuplot_script"
readonly GNUPLOT_SCRIPT_EXT=".gpscr"

show_version()
{
	echo -e "\
Meteo v$VERSION \n\
Packaged under Cygwin (9.0-1) \n\
This is free bash script: you are free to change and redistribute it. \n\
Copyright (C) 2023 \n\
Written by Samantha D. and Mathieu B."
}

reorderData() # $1 == input file | $2 == graph type (t1,p3,w,...) | $3 == order algo (tab, abr, avl) | $4 == -r(everse)
{
	SECONDS=0
	echo $PROG_C_APP -f $1 -o $PROG_C_OUTPUT_FILE_BASE -g $2 -a $3 $4
	echo "started"
	     $PROG_C_APP -f $1 -o $PROG_C_OUTPUT_FILE_BASE -g $2 -a $3 $4
	echo -e "\ntime spent ${SECONDS}s"
	echo -e "sorted meteo:\n"
	head -n5 $PROG_C_OUTPUT_FILE_BASE$GNUPLOT_DATA_EXT
	echo -e "...\n"
}
#~ EndOf CODE C init & functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#~ GNUPLOT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
drawGnuPlot() # $1 == graph type (t1,p3,w,...)
              # $2 == column number for xRange
              # $3 == column name   for xRange
              # $4 == column number for yRange
              # $5 == column name   for yRange
              # $6 == gnuplot graph type
{
#	echo  "cat $PROG_C_OUTPUT_FILE_BASE$GNUPLOT_DATA_EXT | cut -d ';' -f$2 | sort -g | sed -n '1p;\$p'"
#  xRange=($(cat $PROG_C_OUTPUT_FILE_BASE$GNUPLOT_DATA_EXT | cut -d ';' -f$2 | sort -g | sed -n '1p;$p'))
##	xmin=$((${xRange[0]}))
##	xmax=$((${xRange[1]}))
#	xmin=${xRange[0]}
#	xmax=${xRange[1]}

#	echo  "cat $PROG_C_OUTPUT_FILE_BASE$GNUPLOT_DATA_EXT | cut -d ';' -f$4 | sort -g | sed -n '1p;\$p' | cut -d '.' -f1"
#	yRange=($(cat $PROG_C_OUTPUT_FILE_BASE$GNUPLOT_DATA_EXT | cut -d ';' -f$4 | sort -g | sed -n '1p;$p' | cut -d '.' -f1))
#	ymin=$((${yRange[0]} -5))
#	ymax=$((${yRange[1]} +5))
	
	echo "set output './photos/curve_$1.png'"                                                  > $GNUPLOT_SCRIPT
	echo "set terminal png size 800,500 enhanced background rgb 'white'"             >> $GNUPLOT_SCRIPT
#	echo "set output 'curve.pdf'"                                                     > $GNUPLOT_SCRIPT
#	echo "set terminal pdf size 40,25 enhanced background rgb 'white'"               >> $GNUPLOT_SCRIPT
  echo "set style line 1 lt 1 lw 1.5 pt 3 linecolor rgb '#2b63ff'"                 >> $GNUPLOT_SCRIPT
	echo "set autoscale"                                                             >> $GNUPLOT_SCRIPT
	echo "set xtic auto"                                                             >> $GNUPLOT_SCRIPT
	echo "set ytic auto"                                                             >> $GNUPLOT_SCRIPT
	echo "set title 'Gnuplot curve for $5 graph type with mode $1'"                  >> $GNUPLOT_SCRIPT
	echo "set xlabel '$3'"                                                           >> $GNUPLOT_SCRIPT
  echo "set ylabel '$5'"                                                           >> $GNUPLOT_SCRIPT
	if [ "$1" == "t2" ] || [ "$1" == "p2" ]; then
    echo "set xdata time"                                                          >> $GNUPLOT_SCRIPT
    echo "set timefmt '%Y-%m-%dT%H:%M:%S+01:00'"                                   >> $GNUPLOT_SCRIPT
    echo "set format x '%Y-%m-%dT%H:%M:%S+01:00'"                                  >> $GNUPLOT_SCRIPT
#  else
#	  echo "set xr [$xmin:$xmax]"                                                    >> $GNUPLOT_SCRIPT
  fi
#	echo "set yr [$ymin:$ymax]"                                                      >> $GNUPLOT_SCRIPT
	echo "set nokey"                                                                 >> $GNUPLOT_SCRIPT
	echo "set datafile separator ';'"                                                >> $GNUPLOT_SCRIPT
  echo "FILES = system('ls -1 $PROG_C_OUTPUT_FILE_BASE*$GNUPLOT_DATA_EXT')"        >> $GNUPLOT_SCRIPT
	if [ "$6" == "yerrorlines" ]; then
	  echo "plot for [datafile in FILES] datafile using $2:$4:2:3 with yerrorlines"  >> $GNUPLOT_SCRIPT
	else # default
	  echo "plot for [datafile in FILES] datafile using $2:$4 with linespoints ls 1" >> $GNUPLOT_SCRIPT
	fi
	gnuplot $GNUPLOT_SCRIPT
#	rm $GNUPLOT_SCRIPT
}
#~ EndOf GNUPLOT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


TEMP=`getopt -o wmht:p:FGSAOQd:f: --long wind,moisture,height,temperature:,pressure:,France,Guyana,SaintPierre,WestIndies,IndianOcean,Antarctic,avl,abr,tab,help,date:,file: -- "$@"`
eval set -- "$TEMP"

ABR=0
AVL=0
TAB=0

if [ "$1" = "--help" ]; then
    case $1 in
        --help ) 
             echo "Usage:
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
                    --abr : sorting by binary search tree;
                    
                Exit values :
                    - $SUCCESS : success
                    - $ERR_OPTIONS : issue with options
                    - $ERR_DATA_IN : issue with input data file
                    - $ERR_FILES_OUT : issue with output files
                    - $ERR_INTERNAL : internal error

                --help  affiche l'aide
                
                Exemples:
                    - ./main.sh -f $METEO_RESOURCES_PATH/data.csv -t1
             "
             exit
             ;;
        * )
             echo "Invalid option: $1"
             exit 1
    esac
    shift
fi

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

while true; do
    case "$1" in
        -w|--wind)
            wind=1
            Options=$((Options+1))
            shift
            ;;
        -m|--moisture)
            moisture=1
            Options=$((Options+1))
            shift
            ;;

        -h|--height)
            height=1
            Options=$((Options+1))
            shift
            ;;
        -t|--temperature)
            case $2 in
                1)
                    t1=1
                    Options=$((Options+1))
                    shift 2
                    ;;
                2)
                    t2=1
                    Options=$((Options+1))
                    shift 2
                    ;;
                3)
                    t3=1
                    Options=$((Options+1))
                    shift 2
                    ;;
                *)
                    echo "Invalid -t option argument: $2
                    Please complete the option with 1, 2 or 3" >&2
                    exit 1
            esac
            ;;
        -p|--pressure)
            case $2 in
                1)
                    p1=1
                    Options=$((Options+1))
                    shift 2
                    ;;
                2)
                    p2=1
                    Options=$((Options+1))
                    shift 2
                    ;;
                3)
                    p3=1
                    Options=$((Options+1))
                    shift 2
                    ;;
               
                *)
                    echo "Invalid -p option argument: $2
                    Please complete the option with 1, 2 or 3" >&2
                    exit 1
            esac
            ;;
        -f|--file)
            input_files="$2"
            shift 2
            ;;
        -F|--France)
            france_corse=2
            Region=$((Region+1))
            shift
            ;;
        -G|--Guyana)
            guyana=2
            Region=$((Region+1))
            shift
            ;;
        -S|--SaintPierre)
            pierre_miquelon=2
            Region=$((Region+1))
            shift
            ;;
        -A|--WestIndies)
            antilles=2
            Region=$((Region+1))
            shift
            ;;
        -O|--IndianOcean)
            indien=2
            Region=$((Region+1))
            shift
            ;;
        -Q|--Antarctic)
            antarctique=2
            Region=$((Region+1))
            shift
            ;;
        -d|--date)
             case $2 in
                1)
                    start_date=$(date -d "${dates[0]}" +%s)
                    shift 2
                    ;;
                2)
                   end_date=$(date -d "${dates[1]}" +%s)
                    shift 2
                    ;;
                *)
                    echo "Invalid -p option argument: $2
                    Please complete the option with 1, 2 or 3" >&2
                    exit 1
            esac
            ;;
        --abr)
            ABR=1
            
            shift
            ;;
        --avl)
            AVL=1
            shift
            ;;
        --tab)
            TAB=1
            shift
            ;;
      
        --)
            shift
            break
            ;;
        *)
            echo "Invalid option: $1"
            exit 1
            ;;

       
  esac
done

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Dates (non working)~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  #if [ -z "$start_date" ] || [ -z "$end_date" ]; then             #determine the temporal frame
   # echo "Option -d is required with two dates in the format AAAA-MM-JJ" >&2
   # exit 1
  #fi

 # awk -F, -v start="$start_date" -v end="$end_date" '
  # BEGIN {
  #    "date -d \"" start "\" +%s" | getline start_ts
  #    "date -d \"" end "\" +%s" | getline end_ts
  # }
  #  function convert_date(date, t, y, m, d, H, M, S, sign, offset_h, offset_m) {
   #  split(substr(date, 20), sign, "+")
   #  split(sign[2], offset, ":")
   #   offset_h = (sign[2] == "+") ? offset[1] : -offset[1]
   #   offset_m = (sign[2] == "+") ? offset[2] : -offset[2]
  #   t = mktime(sprintf("%s %s %s %s %s %s", substr(date, 1, 4), substr(date, 6, 2), substr(date,   9, 2), substr(date, 12, 2), substr(date, 15, 2), substr(date, 18, 2))) + (offset_h * 3600) +       (offset_m * 60)
   #   return t >= start_ts && t <= end_ts
   # }
  # convert_date($1) {print}
  #' $METEO_RESOURCES_PATH/data.csv > $METEO_RESOURCES_PATH/meteo.csv || { echo "awk failed" >&2;    # exit 1; }

  #echo "Start date: $(date -d @$start_timestamp)"
  #echo "End date: $(date -d @$end_timestamp)"


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


if [[ -n "$input_files" ]] ; then           #verify if there is an input files specified
  echo "Input file specified."
else 
  echo "Error $ERR_OPTIONS - Input File name is mandatory - please take a look to help usage" && exit $ERR_OPTIONS
  exit 
fi



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


echo "number of filtres chosen : $Options"        #count the number of filters chosen
echo "number of régions chosen : $Region"       #count the number of areas chosen
echo "number of dates chosen : $Dates"           #count of which type of time slot chosen


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# Choice of sort and communication with C Program
algo="avl"
if [ $ABR -eq 1 ]; then
  algo="abr"
elif [ $TAB -eq 1 ]; then
  algo="tab"
fi
[ ! -f $PROG_C_APP ] && make all
[ ! -f $PROG_C_APP ] && echo "Error $ERR_INTERNAL - missing program file $PROG_C_APP - aborted" && exit $ERR_INTERNAL


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
tail -n +2 $METEO_RESOURCES_PATH/data.csv > $METEO_RESOURCES_PATH/meteo.csv  #remove the letterhead

# Choice of filter and communication with C Program

if [[ "$Options" -lt 1 ]] ; then
  echo "Error $ERR_OPTIONS - Graph options are mandatory - please take a look on help usages" && exit $ERR_OPTIONS
fi

if [[ "$Options" -gt 0 ]] ; then
	if [[ "$wind" -eq 1 ]] ; then
    # need ordering by StationId and need Wind direction, speed and coordonate
		cut -d ';' -f 1,4,5,10 $METEO_RESOURCES_PATH/meteo.csv > $METEO_RESOURCES_PATH/meteo_wind.csv
    reorderData "$METEO_RESOURCES_PATH/meteo_wind.csv" "w" $algo
		
  fi
	if [[ "$moisture" -eq 1 ]] ; then
		cut -d ';' -f 1,6,10 $METEO_RESOURCES_PATH/meteo.csv > $METEO_RESOURCES_PATH/meteo_moisture.csv
    reorderData "$METEO_RESOURCES_PATH/meteo_moisture.csv" "m" $algo -r
		
  fi
	if [[ "$height" -eq 1 ]] ; then
		cut -d ';' -f 1,10,14 $METEO_RESOURCES_PATH/meteo.csv > $METEO_RESOURCES_PATH/meteo_height.csv
    reorderData "$METEO_RESOURCES_PATH/meteo_height.csv" "h" $algo -r
		
  fi
	if [[ "$t1" -eq 1 ]] ; then
    # need ordering by StationId and need Temperature C°

		cut -d ';' -f 1,11 $METEO_RESOURCES_PATH/meteo.csv > $METEO_RESOURCES_PATH/meteo_temp1.csv
    reorderData "$METEO_RESOURCES_PATH/meteo_temp1.csv" "t1" $algo
    drawGnuPlot "t1" 1 "station ids" 4 "temperatures" "yerrorlines"
		
  fi
	if [[ "$t2" -eq 1 ]] ; then
    # need ordering by DateTime and need Temperature C°

		cut -d ';' -f 2,11 $METEO_RESOURCES_PATH/meteo.csv > $METEO_RESOURCES_PATH/meteo_temp2.csv
    reorderData "$METEO_RESOURCES_PATH/meteo_temp2.csv" "t2" $algo
    drawGnuPlot "t2" 1 "date time" 4 "temperatures" "lines"
		
  fi
	if [[ "$t3" -eq 1 ]] ; then
    # need ordering by DateTime + StationId and need Temperature C°

		cut -d ';' -f 2,1,11 $METEO_RESOURCES_PATH/meteo.csv > $METEO_RESOURCES_PATH/meteo_temp3.csv
    reorderData "$METEO_RESOURCES_PATH/meteo_temp3.csv" "t3" $algo
		
  fi
	if [[ "$p1" -eq 1 ]] ; then
    # need ordering by StationId and need Pressure

		cut -d ';' -f 1,7 $METEO_RESOURCES_PATH/meteo.csv > $METEO_RESOURCES_PATH/meteo_press1.csv
    reorderData "$METEO_RESOURCES_PATH/meteo_press1.csv" "p1" $algo
    drawGnuPlot "p1" 1 "station ids" 4 "pressures" "yerrorlines"
		
  fi
	if [[ "$p2" -eq 1 ]] ; then
    # need ordering by DateTime and need Temperature C°

		cut -d ';' -f 2,7 $METEO_RESOURCES_PATH/meteo.csv > $METEO_RESOURCES_PATH/meteo_press2.csv
    reorderData "$METEO_RESOURCES_PATH/meteo_press2.csv" "p2" $algo
    drawGnuPlot "p2" 1 "date time" 4 "pressures" "lines"
		
  fi
	if [[ "$p3" -eq 1 ]] ; then
    # need ordering by DateTime + StationId and need Temperature C°
		cut -d ';' -f 2,1,7 $METEO_RESOURCES_PATH/meteo.csv > $METEO_RESOURCES_PATH/meteo_press3.csv
    reorderData "$METEO_RESOURCES_PATH/meteo_press3.csv" "p3" $algo
		
  fi
	
fi

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Choice of region and communication with C Program

if [[ "$Region" -lt 1 ]] ; then     #if not any regions are chosen, all the regions are classify
  cut -d ';' -f 1 $METEO_RESOURCES_PATH/meteo.csv | sort -n > $METEO_RESOURCES_PATH/meteo_station.csv
  echo "Here is all the regions"
fi
if [[ "$Region" -gt 1 ]] ; then
  echo "Too much regions"
  exit 
fi
if [[ "$Region" -eq 1 ]] ; then
  cut -d ';' -f 1 $METEO_RESOURCES_PATH/meteo.csv | sort -n > $METEO_RESOURCES_PATH/meteo_station.csv
  if [[ "$france_corse" -eq 2 ]] ; then
    awk -F ";" '$1 <=59 999' $METEO_RESOURCES_PATH/meteo_station.csv > $METEO_RESOURCES_PATH/meteo_station_FR_COR.csv
  fi
   if [[ "$guyana" -eq 2 ]] ; then
    awk -F ";" '$1 >= 81 000 && $1 <=900 000' $METEO_RESOURCES_PATH/meteo_station.csv > $METEO_RESOURCES_PATH/meteo_station_GUY.csv
  fi
   if [[ "$pierre_miquelon" -eq 2 ]] ; then
    awk -F ";" '$1 >= 71 000 && $1 <= 77 999' $METEO_RESOURCES_PATH/meteo_station.csv > $METEO_RESOURCES_PATH/meteo_station_StP-M.csv
  fi
  if [[ "$antilles" -eq 2 ]] ; then
    awk -F ";" '$1 >= 78 000 && $1 <= 79 999' $METEO_RESOURCES_PATH/meteo_station.csv > $METEO_RESOURCES_PATH/meteo_station_ANTI.csv
  fi
  if [[ "$antarctique" -eq 2 ]] ; then
    awk -F ";" '$1 >= 61 000 && $1 <= 61 999' $METEO_RESOURCES_PATH/meteo_station.csv > $METEO_RESOURCES_PATH/meteo_station_ANTA.csv
  fi
  if [[ "$indien" -eq 2 ]] ; then
    awk -F ";" '$1 >= 67 000 && $1 <= 69 999' $METEO_RESOURCES_PATH/meteo_station.csv > $METEO_RESOURCES_PATH/meteo_station_IND.csv
  fi
fi

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#communication with the C Program

if ! which ./meteo_parser > /dev/null; then
 gcc -o ./meteo_parser meteo_parser_main.c
 if [ $? -ne 0 ]; then
   echo "Compilation failed, please check the source code"
     exit 1
   fi
fi


