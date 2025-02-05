#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <stdio.h>
using namespace std ;

//for coordinate axes
#define HEIGHT 20
#define WIDTH 25
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
//ENUMs
enum Direction{ no_dir, d_up, d_right, d_down, d_left };
enum SnakePart{ no_part, s_body, s_tail, s_head };
//GLOBAL VARIABLES
SnakePart positions[ HEIGHT * WIDTH + 1] ;
Direction directions[ HEIGHT * WIDTH + 1] ;

// Function Declarations

int directionChangePossible( Direction from, Direction to){
	if(from == Direction(no_dir) || to == Direction(no_dir)){
		return 0 ;
	}
	if(from == d_right && to == d_left ){
		return 0 ;
	}
	if(from == d_left && to == d_right ){
		return 0 ;
	}
	if(from == d_up && to == d_down ){
		return 0 ;
	}
	if(from == d_down && to == d_up ){
		return 0 ;
	}
	return 1 ;	
}

class Point{
		void setUniIndex(){
			uniIndex = (lineNo - 1)*width + fromLeft  ;
		}
		void setCoords(){
			fromLeft = ( uniIndex % width == 0 ) ? width : uniIndex % width ;
			lineNo = ( uniIndex - fromLeft ) / width + 1 ;
		}
	public:
		int lineNo, fromLeft, uniIndex ;
		static int height, width ;
//		Direction direction ;
		Point(){
			lineNo = -1 ;
			fromLeft = -1 ;	
			uniIndex = -1 ;	
//			direction = no_dir ;
//			isValid() ;
		}
		Point(int line, int left ){
			lineNo = line ;
			fromLeft = left ;
			uniIndex = (lineNo - 1)*width + fromLeft  ;
			isValid() ;
		}
		Point(int uni_index ){
			uniIndex = uni_index ;
			fromLeft = ( uniIndex % width == 0 ) ? width : uniIndex % width ;
			lineNo = ( uniIndex - fromLeft ) / width + 1 ;
			isValid() ;
		}
		static void showDirectionArray(){
			int i ;
			for(i = 1 ; i <= height * width ; i++){
				cout << "\n[" << i << "] => " << Point(i).getDirection() ;
			}
		}	
		static void resetDirections(){
			int i ;
			for(i = 1 ; i <= height * width ; i++){
				directions[ i ] = Direction(no_dir) ;
			}			
		}	
		Direction getDirection( ){
			if(isInsideBounds()){
				return directions[ uniIndex ] ;
			}
			cout << ", so returning no_dir at this point.\n" ;
			return Direction( no_dir ) ;
		}
		void setDirection( Direction dir ){
			if( isInsideBounds()){
				directions[ uniIndex ] = dir ;
				return ;
			}
			cout << ", so setting no_dir at this point.\n" ;
			directions[uniIndex] = Direction(no_dir) ;			
		}		
		static Point above( Point a){
			if(a.isInsideBounds()){
				return Point( a.lineNo - 1, a.fromLeft) ;
			}
			cout << ", so cant find any point above. Returning itself, i.e" << a.toString() ;
		}
		static Point below( Point a){
			if(a.isInsideBounds()){
				return Point( a.lineNo + 1, a.fromLeft) ;
			}
			cout << ", so cant find any point below. Returning itself, i.e" << a.toString() ;
		}
		static Point atRightOf( Point a){
			if(a.isInsideBounds()){
				return Point( a.lineNo , a.fromLeft + 1) ;
			}
			cout << ", so cant find any point at right. Returning itself, i.e" << a.toString() ;
		}
		static Point atLeftOf( Point a){
			if(a.isInsideBounds()){
				return Point( a.lineNo , a.fromLeft - 1 ) ;
			}
			cout << ", so cant find any point at left. Returning itself, i.e" << a.toString() ;
		}
//		static Point aheadByDirection( Point a, Direction dir){
//			if(dir != no_dir ){
//				switch( dir ){
//					case Direction(d_right):
//						return Point::atRightOf( a ) ;
//						break ;
//					case Direction(d_left):
//						return Point::atLeftOf( a ) ;
//						break ;
//					case Direction(d_up):
//						return Point::above( a ) ;
//						break ;
//					case Direction(d_down):
//						return Point::below( a ) ;
//						break ;
//					default:
//						cout << "Cant find any point ahead of " << a.toString("", ", so returning itself.\n") ;
//						return a ;				
//				}			
//			}else{
//				cout << "Cant find any point ahead of " << a.toString("", ", since no_dir is passed as direction. so returning itself.\n") ;
//				return a ;
//			}
//		}
		static Point aheadOf( Point a, Direction dir = no_dir){
			if(dir == no_dir ) dir = a.getDirection() ;
 			if(a.isValidPart()){
				switch( dir ){
					case Direction(d_right):
						return Point::atRightOf( a ) ;
						break ;
					case Direction(d_left):
						return Point::atLeftOf( a ) ;
						break ;
					case Direction(d_up):
						return Point::above( a ) ;
						break ;
					case Direction(d_down):
						return Point::below( a ) ;
						break ;
					default:
						cout << "Cant find any point ahead of " << a.toString("", ", so returning itself.\n") ;
						return a ;				
				}
			}
			cout << ", So cant find any point ahead of " << a.toString("", ", so returning itself.\n") ;
			return a ;
		}
		int hasValidDirection(){
			if(getDirection() == Direction(no_dir)){
				cout << "Invalid Part at " << toString() << ", because there is no direction data at this point.\n" ;
				return 0 ;
			}
			return 1 ;			
		}
		int hasValidPart(){
			if(positions[ uniIndex ] == SnakePart(no_part)){
				cout << "Invalid Part at " << toString() << ", because there is no snakePart data at this point.\n" ;
				return 0 ;
			}	
			return 1 ;		
		}
		int isValidPart(){
			if(hasValidPart() && hasValidDirection() && isInsideBounds()){
				return 1 ;
			}
			return 0 ;
		}
		static SnakePart getSnakePartAt( Point p){
			if(p.isInsideBounds()){
				return positions[ p.uniIndex ] ;
			}
			//for error
			cout << ", so cant get snake part and returning no_part." ;
			return SnakePart(no_part );
		}				
		int isInBorder(){
			if(lineNo >= 0 && lineNo <= height + 1  && fromLeft >= 0 && fromLeft <= width + 1 ){
				if(lineNo == 0 || lineNo == height + 1  || fromLeft == 0 || fromLeft == width + 1 ){
					return 1 ;
				}
				return 0 ;
			}else{
				cout << "Invalid Point ( " << lineNo << ", " << fromLeft  << ") for border touch check." ;
				return 0 ;
			}			
		}	
		int containsSnake(){
			if(isInsideBounds()){
				int i ;
				for( i = 1 ; i <= HEIGHT * WIDTH ; i++){
					if(positions[ uniIndex ] == SnakePart(s_body) ){
//						cout << positions[uniIndex] ;
						return 1 ;
					}
				}
				if( i == HEIGHT * WIDTH + 1){
					return 0 ;
				}
			}
			cout << ", so cant check if this point contains snake." ;
			return 0 ;
		}

//		int isValidSnakePart(){
//			if(isValid()){
//				int uniIndex = Graph::coordsToIndex(lineNo, fromLeft) ;
////				Point 
//				SnakePart part = positions[uniIndex] ;
//				Point atRight = Point::atRightOf( this ) , atLeft = Point::atLeftOf( this ) , above = Point::above( this ), below = Point::below( this ) ;
//				int bodyAtRight = getSnakePartByPoint(atRight) == SnakePart(s_body) ;
//				int bodyAtLeft = getSnakePartByPoint(atLeft) == SnakePart(s_body) ;
//				if(part == SnakePart(no_part)){
//					return 0 ;
//				}
//				if(part == SnakePart(s_tail) || part == SnakePart(s_head)){
//					if(		getSnakePartByPoint(atRight) == SnakePart(s_body) 
//						||  getSnakePartByPoint(atLeft) == SnakePart(s_body) 
//						||  getSnakePartByPoint(above) == SnakePart(s_body) 
//						||  getSnakePartByPoint(below) == SnakePart(s_body) 
//					){
//						return 1 ;
//					}
//					return 0 ;
//				}
//				if(part == SnakePart(s_body)){
//					
//				}
//			}
//		}	
		void show(){
			cout << " ( " << lineNo << ", " << fromLeft << " ) ";
		}
		string toString(string pre = " ", string post = " "){
			string str = "" ;
			str = str + pre + "( " + to_string(lineNo) + ", " + to_string(fromLeft) + " ) => [ " + to_string(uniIndex) + " ]" + post ;
			return str ;
		}
		// Point (0, WIDTH + 1 ) is Valid
		int isValid(){
			if(lineNo >= 0 && lineNo <= height + 1 && fromLeft >= 0 && fromLeft <= width + 1 ){
				return 1 ;
			}
			cout << toString("Point", " is not valid.") ;
			return 0 ;
		}
		// Point (0, WIDTH + 1 ) is not inside bounds
		int isInsideBounds(){
			if(lineNo > 0 && lineNo <= height && fromLeft > 0 && fromLeft <= width ){
				return 1 ;
			}
			cout << "Point " << toString() << " is not within bounds (" << height << ", " << width << " )." ;
			return 0 ;			
		}	
		operator == ( Point a){
			return uniIndex == a.uniIndex ;
		}	
		
};
int Point::height = HEIGHT ;
int Point::width = WIDTH ;

//Point activePositions[ HEIGHT * WIDTH + 1] ;

class Graph ;

class Snake{
	public:
		int length = 3 ;
		Point headAt, tailAt ;
//		Point activePoints[ HEIGHT * WIDTH + 1] ;
		string  bodyLetter, tailLetter ;
		Direction activeDirection = d_right ;
		Snake(){
			headAt = Point(5, 5) ;
			tailAt = Point(5, 2) ;
			resetParts() ;
			bodyLetter = "*" ;
			tailLetter = "+" ;
//			addPart(Point(5, 2), Direction(d_right), SnakePart(s_tail)) ;
		}
		void removePart( Point at ){
			directions[at.uniIndex] = Direction(no_dir) ;
			positions[at.uniIndex] = SnakePart(no_part) ;
		}
		void addPart( Point At, Direction dir, SnakePart part = s_body ){
			if(At.isInsideBounds()){
				if(!( part == SnakePart(no_part) || dir == Direction(no_dir))){
					positions[ At.uniIndex ] = part ;
					At.setDirection( dir ) ;
				}else{
					cout << "Either direction or part is empty. Cant add this data at " << At.toString("Point") <<".\n" ;
					return ;
				}
//				Graph::setDirection(dir,  At ) ;
				if(part == SnakePart(s_head) ){
					headAt = At ;
				}
				if(part == SnakePart( s_tail )){
					tailAt = At ;
				}
				return ;
			}
			cout << ", so cant add part at this point.\n" ;
		}
		void resetParts(){
			int i ;
			for( i = 1 ; i <= Point::height * Point::width ; i++){
				positions[i] = SnakePart(no_part) ;
			}
		}
		void moveOneStep( Direction dir ){
			if(directionChangePossible(headAt.getDirection(), dir)){
				Point newHeadAt = Point::aheadOf(headAt, dir) ;
				Point newTailAt = Point::aheadOf( tailAt ) ;
				removePart( tailAt ) ;
				addPart(headAt, headAt.getDirection(), s_body ) ;
				
				addPart(newHeadAt, dir , s_head) ;
				headAt = newHeadAt ;
				tailAt = newTailAt ;
			}else{
				cout << "cant move in this direction.\n";
			}
		}
};


class Graph{
	string inputLetters[HEIGHT * WIDTH + 1] ;
	public:

	static int height, width;
	Snake snake;
	Direction d_resetter ;
	int paddingLeft ;
	string borderH, borderV, default_content, padding, paddingLeftText, resetter ;
		Graph(){
			height = HEIGHT ;
			width = WIDTH ;
			paddingLeft = 35 ;
			borderH = "=" ;
			borderV = "||" ;
			default_content = " " ;
			padding = " " ;
			resetter = "`";
			d_resetter = no_dir ;
			paddingLeftText = getStringWith(paddingLeft, padding) ;
//			cout << inputLetters == NULL ;
			resetInputs() ;
			resetDirections() ;
		}
		void showInputArray(){
			int i ;
			for(i = 1 ; i <= height * width ; i++){
				cout << "\n[" << i << "] => " << getInput(i) ;
			}
		}		
		void show(){
			system("CLS") ;
			cout << endl << endl ;
			int i ;
			for( i = 0 ; i <= height + 1 ; i++ ){
				showLine(i) ;
			}
			resetInputs() ;
		}
		void putRegularly(string str, Point startingPoint ){
			if( ! startingPoint.isValid() ) return ;
			int i, j  ;
			string sub ;
			for( i = startingPoint.uniIndex, j = 0 ; j < str.length() ; i++, j++ ){
				sub = str.at(j) ;
				put(sub, Point(i)) ;
			}
		}
		void put( string str, Point P){
			if(str.length() != 1){
				cout << "Error, can put single letter only.\n" ;
				return ;
			}
			if(str.compare(resetter) == 0){
				cout << "Error, '" << resetter << "' cannot be added in space.\n" ;
				return ;
			}
			if(P.isInsideBounds()){
				setInput(str, P) ;	
				return ;		
			}
			cout << ", so cant put '" << str << "' at that point.\n" ;
		}
		
		void plotSnake(){
			int i ;
			resetInputs() ;
			Point currentPoint = snake.tailAt ;
			while( 1 ){
				if(!currentPoint.isInsideBounds()){
					show() ;
					break ;
				}
				string inputLetter = "~" ;
				// head may not have direction, but direction is compulsory for all other points
//				if(!(currentPoint == snake.headAt) ){
					if(currentPoint.getDirection() == Direction(no_dir)){
						cout << "No direction data at " << currentPoint.toString("Point", ", so stopping plotting from this point.\n") ;
						break ;
					}
//				}
				if(currentPoint == snake.headAt ){
					switch ( currentPoint.getDirection() )	{
						case Direction( d_up ):
							inputLetter = "^" ;
							break ;
						case Direction(d_right):
							inputLetter = ">" ;
							break ;
						case Direction (d_left ):
							inputLetter = "<" ;
							break ;
						case Direction (d_down)	:
							inputLetter = "+" ;
							break ;		
					}
				}else if(currentPoint == snake.tailAt){
					inputLetter = snake.tailLetter ;
				}else{
					// for body
					inputLetter = snake.bodyLetter ;
				}
				setInput( inputLetter, currentPoint ) ;
				if(currentPoint == snake.headAt){
					show() ;
					break ;
				}
				currentPoint = Point::aheadOf( currentPoint );
			}
		}
	private:		
		void setInput(string str, Point at){
			if(at.isInsideBounds() && str.length() == 1 ){
				inputLetters[ at.uniIndex ] = str ;
				return ;
			}
			cout << ", so cant set input at this point.\n" ;
		}
		string getInput( Point at){
			if(at.isInsideBounds() ){
				return inputLetters[ at.uniIndex ] ;
			}
			cout << ", so cant get input from this point, returning default resetter.\n" ;
			return resetter ;
		}
		void resetInputs(){
			int i ;
			for( i = 1 ; i <= height * width  ; i++){
				setInput(resetter, Point(i)) ;
			}
		}
		void resetDirections(){
			int i ;
			for( i = 1 ; i <= height * width  ; i++){
				Point(i).setDirection(d_resetter) ;
//				setDirection(d_resetter , Point(i)) ;
			}
		}		
		void showLine(int lineNumber ){
			//paddingLeft
			string lineOut = paddingLeftText ;
			//border
			lineOut.append(borderV) ;
			//top border and bottom border
			if((lineNumber == 0) || (lineNumber == height + 1)) {
				lineOut.append(getStringWith(width * 2, borderH)) ;	
			}else{
			//line specific options
				int i  ;
				for( i = (lineNumber - 1)*width + 1 ; i <= lineNumber*width ; i++){
	//					cout << i << ", " ;
//					string toDisplay = *(inputLetters + i) ;
					if(getInput(i).compare(resetter) == 0){
						lineOut.append(default_content) ;
					}else{
						lineOut.append(getInput(i)) ;
					}
					lineOut.append(" ") ;
				}
			
			}
			//ending border
			lineOut.append(borderV) ;
			//new line at end
			lineOut.append("\n") ;
			cout << lineOut ;
		}
		static string getStringWith(int len, string s){
			int i = 0 ;
			string out ;
			for(i = 0 ; i < len ; i++){
				out.append(s);
			}
			return out ;
		}
};
int Graph::height = HEIGHT ;
int Graph::width = WIDTH ;


main(){
	Point P(5, 4) ;
	Graph G ;
	G.snake.addPart(P, d_down, s_body ) ;
	cout << P.containsSnake() ;
	
}
