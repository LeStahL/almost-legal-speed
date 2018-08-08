# mx 2018, installs and runs the game
#
#
# SINGLE INT NUMBER CHARACTER AS FIRST ARGUMENT TO CHOOSE LEVEL
# CURRENTLY WORKS ONLY FOR LESS THAN 10 LEVELS
#
# .TTF FILE AS SECOND ARGUMENT TO CHOOSE PERSONAL FONT
#
#
# DEFAULT: LEVEL 4, FREESANS.TTF


RED='\033[1;31m'
GREEN='\033[1;32m'
NC='\033[0m'


# INSTALLATION
if [ ! -d "build" ]; then
	echo
	echo -e "${RED}  [WARNING]${NC}"
	echo
	echo ">> Game not installed (correctly)"
	echo ">> Do you want to (re-)install? (Y/N)"
	echo
	read -s var
	while [[ "$var" != "y" && "$var" != "Y" && "$var" != "N" && "$var" != "n" ]]; do
		echo ">> Type 'Y' or 'N'!"
		read -s var
	done

	if [[ "$var" == "y" || "$var" == "Y" ]]; then

		echo -e "${GREEN}  [INSTALLATION STARTED]${NC}"
		echo
		
		mkdir build
		cd build
		cmake ..
		if make; then
			echo
			echo -e "${GREEN}  [INSTALLATION SUCCEEDED]${NC}"
			echo
			echo ">> Installation directory is '/build/' "
			echo
		fi

	else 
		echo 
		echo ">> Bye Bye"
		echo
		exit 1
	fi
		
fi

if [ -d "build" ]; then
	echo
	echo -e "${GREEN}  [WELCOME!]${NC}"
	echo 
fi

# RUNNING THE MOTHERFUCKING GAME

LEVEL="$1"
FONT="$2"

if [ -z "$LEVEL"]; then
	LEVEL="4"
fi

if [ -z "$FONT" ]; then
	FONT="../gfx/fonts/FreeSans.ttf"
fi 

echo Level $LEVEL
echo with Font $FONT

cd build
if ./speed -f $FONT -l ../levels/level0$LEVEL; then
	echo
	echo -e "${GREEN}  [GOODBYE]${NC}"
	echo ">> Thanks for playing"
	echo
else
	echo -e "${RED}  [WARNING]${NC}"
	echo ">> Broken installation"
	echo ">> Please try again after removing 'build' directory"
	echo
	echo ">> Please check that the following 'curl' and 'smfl' are installed:"
	echo ">> try 'sudo apt-get install libcurl4-gnutls-dev libsfml-dev'"
	echo

fi

