#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <SDL.h>
#include <SDL_mixer.h>
using namespace std;

//************ SDL functions *************//
void logSDLError(std::ostream& os, const std::string &msg, bool fatal = false);
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);
void clearRegion(SDL_Renderer* renderer, int x, int y, int width, int height);
//*****************************************//

//****************************** Khai Bao ************************/
const string WINDOW_TITLE = "Sudoku Solve";
const int SCREEN_WIDTH = 1225;
const int SCREEN_HEIGHT = 690;

const int N = 9;
const int soDeDe = 20;
const int soDeTrungBinh = 20;
const int soDeKho = 20;

int board[N][N];
int level = 1;
int countHeart = 0;
int countHint = 0;
int hintArray[1000] = {0};
int timesWrongArray[1000] = {0};
bool isSoundPlaying = true;

int question[N][N] = {0};
int result[N][N] = {0};
int fillArray[N][N] ={0};

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *mainScreenEasy, *mainScreenMedium,*mainScreenHard, *startScreen, *introduction1, *introduction2;
SDL_Texture *correct, *wrong, *success, *tran, *soundOn, *soundOff;
SDL_Texture *number1, *number2, *number3, *number4, *number5, *number6, *number7, *number8, *number9;
SDL_Texture *hint0, *hint1, *hint2, *hint3;
SDL_Texture *heart, *gameover, *congratulation, *niceTry;
Mix_Chunk *click, *rightSound, *wrongSound, *failSound, *successSound, *chooseSound;
Mix_Music *backgroundMusic1, *backgroundMusic2;
SDL_Surface* icon;
//****************************** Khai Bao ************************/


//************ My Functions *************/
void windowIcon();
void load_SDL_and_Images();
void load_SDL_and_Sound();
void unload_SDL_and_Images();
void unload_SDL_and_Sound();
void showGame(int board[N][N]);
bool isSafe(int board[N][N], int row, int col, int num);
bool solveSudoku(int board[N][N], int row, int col);
bool isSolvedCompletely(int grid[N][N]);
bool isNewGame(int x, int y);
void successSolve(int question[N][N]);
void inputAnswer(int question[][N], int result[][N], int fillAray[][N]);
int computeNumber(int x, int y);
int getPlayerGuess();
pair<int,int> getPos();
void dealWrong(int timesWrong);
void dealRight(int timesWrong);
void gameOver();
void drawHeart(int timesWrong);
void drawHint(int hints);
void play(int question[][N], int result[][N], int fillAray[][N]);
void chonDeDe();
void chonDeTrungBinh();
void chonDeKho();
void newGame();
void showAnswer();
void getOpinion();
void Introduction1();
void Introduction2();
void prepare();
void soundWhenOpen();
void drawPos(int x, int y);
void drawSound();
void chosenNumber(int question[][N], int number);
void fillAnswer(int board[][N]);
void exitApp();
/*********************************************/



int main(int argc, char* argv[]) {

    load_SDL_and_Images();
    load_SDL_and_Sound();

    windowIcon();
    soundWhenOpen();
    prepare();

    exitApp();
    return 0;
}

void exitApp(){
    unload_SDL_and_Images();
    unload_SDL_and_Sound();
    exit(1);
}

/****************** Set Window Icon ***********************/
void windowIcon(){
    icon = SDL_LoadBMP("SudokuImages/windowIcon_.bmp");
    if (icon == nullptr) {
        cout << "SDL_LoadBMP failed: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        exit(1);
    }
    SDL_SetWindowIcon(window, icon);
    SDL_RenderPresent(SDL_GetRenderer(window));
}
/****************** Set Window Icon ***********************/


/****************** Using sounds ***********************/
void soundWhenClick(){
    Mix_PlayChannel(-1, click, 0);
    while (Mix_Playing(-1) != 0);
}
void soundWhenCorrect(){
    Mix_PlayChannel(-1, rightSound, 0);
    while (Mix_Playing(-1) != 0);
}
void soundWhenWrong(){
    Mix_PlayChannel(-1, wrongSound, 0);
    while (Mix_Playing(-1) != 0);
}
void soundWhenSuccess(){
    Mix_PlayChannel(-1, successSound, 0);
    while (Mix_Playing(-1) != 0);
}
void soundWhenFail(){
    Mix_PlayChannel(-1, failSound, 0);
    while (Mix_Playing(-1) != 0);
}
void soundWhenOpen(){
    Mix_PlayMusic(backgroundMusic1, -1);
}
void soundWhenPlaying(){
    Mix_PlayMusic(backgroundMusic2, -1);
}
void soundWhenChoose(){
    Mix_PlayChannel(-1, chooseSound, 0);
    while (Mix_Playing(-1) != 0);
}
void drawSoundPlayingOff(){
    clearRegion(renderer,55,92,75,80);
    renderTexture(soundOff, renderer, 60, 90, 60, 85);
    SDL_RenderPresent(renderer);
}
void drawSoundPlayingOn(){
    clearRegion(renderer,55,92,75,80);
    renderTexture(soundOn, renderer, 57, 95, 65, 80);
    SDL_RenderPresent(renderer);
}
bool soundOnOff(int x, int y){
    return (x >= 58 && x <= 124 && y >= 90 && y <= 176);
}
void drawSound(){
    if (isSoundPlaying) drawSoundPlayingOn();
    else drawSoundPlayingOff();
}
/****************** Using sounds ***********************/



/****************** Make process ***********************/
void prepare(){
    renderTexture(startScreen, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_RenderPresent(renderer);
    getOpinion();
}
bool isStart(int x, int y){
    return (x >= 236 && x <= 503 && y >= 258 && y <= 355);
}
bool isExit(int x, int y){
    return (x >= 236 && x <= 503 && y >= 513 && y <= 606);
}
bool isIntroduction(int x, int y){
    return (x >= 236 && x <= 503 && y >= 389 && y <= 481);
}
void getOpinion() {
    SDL_Event e;
    bool running = true;

    while (running) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
                exitApp();
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x;
                int y = e.button.y;

                if (isStart(x, y)) {
                    soundWhenPlaying();
                    newGame();
                }
                else if (isIntroduction(x,y)){
                    Introduction1();
                }
                else if (isExit(x, y)) {
                    soundWhenClick();
                    exitApp();
                }
            }
        }
        SDL_Delay(10);
    }
}
bool isContinueInIntroduction1(int x, int y){
    return (x >= 286 && x <= 498 && y >= 584 && y <= 636);
}
void Continue() {
    SDL_Event e;
    bool running = true;

    while (running) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
                exitApp();
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x;
                int y = e.button.y;

                if (isContinueInIntroduction1(x,y)) {
                    soundWhenClick();
                    Introduction2();
                }
            }
        }
        SDL_Delay(10);
    }
}

void Introduction1(){
    soundWhenClick();
    renderTexture(introduction1, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_RenderPresent(renderer);
    Continue();
}

bool isStartInIntroduction2(int x, int y){
    return (x >= 664 && x <= 877 && y >= 530 && y <= 585);
}
bool isExitInIntroduction2(int x, int y){
    return (x >= 664 && x <= 877 && y >= 604 && y <= 656);
}

void StartInIntroduction2(){
    SDL_Event e;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
                exitApp();
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x;
                int y = e.button.y;

                if (isStartInIntroduction2(x,y)){
                    Mix_HaltMusic();
                    soundWhenPlaying();
                    newGame();
                }
                else if (isExitInIntroduction2(x,y)){
                    soundWhenClick();
                    prepare();
                }
            }
        }
        SDL_Delay(10);
    }
}
void Introduction2(){
    renderTexture(introduction2, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_RenderPresent(renderer);
    StartInIntroduction2();
}
/****************** Make process ***********************/



/******************Get question from 1 of 3 files***********************/
void chonDeDe()
{
    srand(time(nullptr));
    int randomNumber = rand() % soDeDe + 1;
    ifstream inputFile("DeDe/dauvao" + to_string(randomNumber) + ".txt"); // Đường dẫn đến file chứa các số
    if (!inputFile) {
        cout << "Failed to open the input file." << endl;
        return;
    }
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            inputFile >> board[i][j];
        }
    }
}
void chonDeTrungBinh()
{
    srand(time(nullptr));
    int randomNumber = rand() % soDeTrungBinh + 1;
    ifstream inputFile("DeTrungBinh/dauvao" + to_string(randomNumber) + ".txt"); // Đường dẫn đến file chứa các số
    if (!inputFile) {
        cout << "Failed to open the input file." << endl;
        return;
    }
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            inputFile >> board[i][j];
        }
    }
}
void chonDeKho()
{
    srand(time(nullptr));
    int randomNumber = rand() % soDeKho + 1;
    ifstream inputFile("DeKho/dauvao" + to_string(randomNumber) + ".txt"); // Đường dẫn đến file chứa các số
    if (!inputFile) {
        cout << "Failed to open the input file." << endl;
        return;
    }
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            inputFile >> board[i][j];
        }
    }
}
/******************Get question from 1 of 3 files***********************/



/****************** Deal with Ending ***********************/
bool isStartInEnd(int x, int y){
    return (x >= 343 && x <= 548 && y >= 508 && y <= 567);
}

bool isExitInEnd(int x, int y){
    return (x >= 672 && x <= 875 && y >= 508 && y <= 567);
}
void opinionInEnd() {
    SDL_Event e;
    bool running = true;

    while (running) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
                exitApp();
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x;
                int y = e.button.y;

                if (isExitInEnd(x,y)) {
                    soundWhenClick();
                    soundWhenOpen();
                    countHeart++;
                    countHint++;
                    prepare();
                }
                else if(isStartInEnd(x,y)){
                    countHeart++;
                    countHint++;
                    soundWhenPlaying();
                    newGame();
                }
            }
        }
        SDL_Delay(10);
    }
}
/****************** Deal with Ending ***********************/



/****************** Deal with Right or Wrong situation ***********************/
void gameOver(){
    SDL_Delay(2000);
    SDL_RenderClear(renderer);
    Mix_HaltMusic();
    renderTexture(gameover, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_RenderPresent(renderer);
    soundWhenFail();
    opinionInEnd();
    exitApp();
}

void successSolve(int question[N][N]) {
    if (isSolvedCompletely(question)) {
        clearRegion(renderer, 855, 510, 240, 42);
        renderTexture(success, renderer, 855, 510, 240, 42);
        SDL_RenderPresent(renderer);
        SDL_Delay(2000);
        Mix_HaltMusic();
        renderTexture(congratulation, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        SDL_RenderPresent(renderer);
        soundWhenSuccess();
        opinionInEnd();
        exitApp();
    }
}

void dealWrong(int timesWrong){
    soundWhenWrong();
    showGame(question);
    fillAnswer(fillArray);
    clearRegion(renderer, 855, 510, 240, 42);
    renderTexture(wrong, renderer, 855, 510, 240, 42);
    for (int i = 0; i < timesWrong; i++){
        clearRegion(renderer, 655, 300 + 95 * i, 80, 80);
    }

    drawHint(hintArray[countHint]);
    drawHeart(timesWrong);
    SDL_RenderPresent(renderer);
}

void dealRight(int timesWrong){
    showGame(question);
    fillAnswer(fillArray);
    drawHint(hintArray[countHint]);
    renderTexture(correct, renderer, 855, 510, 240, 42);
    drawHeart(timesWrong);
    soundWhenCorrect();
    SDL_RenderPresent(renderer);
}

void drawHeart(int timesWrong){
    for (int i = 3-timesWrong; i > 0; i--)
    {
        renderTexture(heart,renderer, 655, 585 - 95 * i, 80, 80);
    }
    SDL_RenderPresent(renderer);
}
/****************** Deal with Right, Wrong, Win, Lose situation ***********************/



/****************** Hint ***********************/
bool isHint(int x, int y){
    return (x >= 660 && x <= 732 && y >= 86 and y <= 155);
}

void drawHint(int hints){
    if (hints == 0) renderTexture(hint3, renderer, 669,170,40,20);
    if (hints == 1) renderTexture(hint2, renderer, 669,170,40,20);
    if (hints == 2) renderTexture(hint1, renderer, 669,170,40,20);
    if (hints == 3) renderTexture(hint0, renderer, 669,170,40,20);
    SDL_RenderPresent(renderer);
}

void dealHint(int question[][N], int result[][N], int fillArray[][N], int row, int col){
     question[row][col] = result[row][col];
     fillArray[row][col] = result[row][col];
     showGame(question);
     fillAnswer(fillArray);
     renderTexture(correct, renderer, 855, 510, 240, 42);
     drawHeart(timesWrongArray[countHeart]);
     drawHint(hintArray[countHint]);
     soundWhenCorrect();
     SDL_RenderPresent(renderer);
}
/****************** Hint ***********************/



/****************** Playing ********************************/
bool easyGame(int x, int y){
    return (x>= 147 and x<=285 and y>=55 and y<=80);
}
bool mediumGame(int x, int y){
    return (x>= 318 and x<=455 and y>=55 and y<=80);
}
bool hardGame(int x, int y){
    return (x>= 485 and x<=623 and y>=55 and y<=80);
}
bool isNewGame(int x, int y) {
    return (x >= 132 and x <= 357 and y >= 597 and y <= 640);
}
bool retire(int x, int y){
    return (x >= 412 && x <= 638 && y >= 597 && y <= 640);
}
void play(int question[][N], int result[][N], int fill[][N]){
    showGame(question);
    fillAnswer(fillArray);
    drawHeart(timesWrongArray[countHeart]);
    drawHint(hintArray[countHint]);
    //pause();
    inputAnswer(question,result,fill);
}

void inputAnswer(int question[][N], int result[][N], int fill[][N]) {
    while (!isSolvedCompletely(question)) {
        pair<int,int> viTri =  getPos();
        int row = viTri.first, col = viTri.second;

        if (question[row][col] == 0) drawPos(row,col);

        else if (question[row][col] != 0) {
            cout << "(" << row + 1 << "," << col + 1 << ")" << " is chosen number" << endl;
            chosenNumber(question,question[row][col]);
        }
        soundWhenChoose();
        cout << "(" << row + 1 << "," << col + 1 << ")" << " ";

        int number = getPlayerGuess();
        cout << number << endl;

        if (number == -1) play(question,result,fill); // bỏ chọn nước đi
        else if (number == 10 and hintArray[countHint] >= 3) play(question,result,fill);//hết gợi ý
        else if (number == 10 and hintArray[countHint] < 3) // dùng gợi ý
        {
            cout << "hint" << hintArray[countHint]+1 << endl;
            hintArray[countHint]++;
            dealHint(question,result,fill,row,col);
            inputAnswer(question,result,fill);
        }

        else if (result[row][col] == number) {
            question[row][col] = number;
            fillArray[row][col] = number;
            dealRight(timesWrongArray[countHeart]);
        }
        else {
            timesWrongArray[countHeart]++;
            dealWrong(timesWrongArray[countHeart]);
            cout  << "Wrong = " << timesWrongArray[countHeart] << endl;
            if (timesWrongArray[countHeart] == 3) gameOver();
        }

    }
    successSolve(question);
}

void drawPos(int x, int y){
    int circleStartX = 133, circleStartY = 81;
    renderTexture(tran, renderer, circleStartX + 55 * y + 3, circleStartY + 55 * x + 3, 65, 65);
    SDL_RenderPresent(renderer);
}
void showGame(int board[N][N]){
    SDL_RenderClear(renderer);
    if (level == 1) renderTexture(mainScreenEasy, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (level == 2) renderTexture(mainScreenMedium, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (level == 3) renderTexture(mainScreenHard, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    drawSound();
    int startX = 125;
    int startY = 75;
    for (int row = 0; row <N; row++ ){
        for (int col = 0; col <N; col++){
            int index = board[row][col];
            if (to_string(index) != "0"){
            string StringOfnumber = "SudokuImages/" + to_string(index)+".bmp";
            SDL_Texture *numberI;
            numberI = loadTexture(StringOfnumber, renderer);
            renderTexture(numberI, renderer, startX+55*col, startY+55*row, 80, 80);
             }
        }
    }
    SDL_RenderPresent(renderer);
}
void fillAnswer(int board[N][N]){
    drawSound();
    int startX = 125;
    int startY = 75;
    for (int row = 0; row <N; row++ ){
        for (int col = 0; col <N; col++){
            int index = board[row][col];
            if (to_string(index) != "0"){
                string StringOfnumber = "SudokuImages/" + to_string(index)+"_.bmp";
                SDL_Texture *numberI;
                numberI = loadTexture(StringOfnumber, renderer);
                renderTexture(numberI, renderer, startX+55*col, startY+55*row, 80, 80);
            }
        }
    }
    SDL_RenderPresent(renderer);
}
void chosenNumber(int question[][N], int number){
    int startX = 125;
    int startY = 75;
    string StringOfnumber = "SudokuImages/" + to_string(number)+"_gr.bmp";
    SDL_Texture *numberI = loadTexture(StringOfnumber, renderer);
    for (int row = 0; row <N; row++ ){
        for (int col = 0; col <N; col++){
            if (question[row][col] == number){
                renderTexture(numberI, renderer, startX+55*col, startY+55*row, 80, 80);
            }
        }
    }
    SDL_RenderPresent(renderer);
}
int computeNumber(int x, int y){
    int tableX0 = 815, tableY0=85, tableX1=1121, tableY1= 365;
    if (x<tableX0 or x>tableX1 or y<tableY0 or y>tableY1) return -1;
    int col = (x-tableX0)/105+1, row=(y-tableY0)/95;
    return (row*3+ col);
}

pair<int,int> computePos(int x, int y){
    int tableX0 = 137, tableY0 = 87, tableX1 = 634, tableY1 = 581;
    if (x < tableX0 or x > tableX1 or y < tableY0 or y > tableY1) return {-1, -1};
    int col = (x - tableX0) / 55, row = (y - tableY0) / 55;
    pair<int, int> toaDo;
    toaDo.first = row;
    toaDo.second = col;
    return toaDo;
}

int getPlayerGuess(){
    int tableX0 = 815, tableY0=85, tableX1=1121, tableY1= 365;
    int number = -1;
    SDL_Event e;

    while (number < 0 || number > N) {
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int x = e.button.x, y = e.button.y;
            if (isHint(x,y)) return 10;
            else if (x<tableX0 or x>tableX1 or y<tableY0 or y>tableY1) return -1;
            else number = computeNumber(x, y);
        }
        if (SDL_PollEvent(&e) == 0) continue;

        if ((e.type == SDL_QUIT) || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
            exitApp();
        }
    }
    return number;
}

pair<int,int> getPos(){
    pair<int,int> num={-1,-1};
    SDL_Event e;
    while (num.first<0 || num.first>N) {
        if ( SDL_WaitEvent(&e) == 0) continue;
        if ((e.type == SDL_QUIT) || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
            exitApp();
        }
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int x = e.button.x, y = e.button.y;
            if(isNewGame(x,y))
            {
                countHeart++;
                countHint++;
                newGame();
            }
            else if(retire(x,y)) showAnswer();
            else if (easyGame(x,y)) {
                level = 1;
                countHeart++;
                countHint++;
                newGame();
            }
            else if(mediumGame(x,y)){
                level = 2;
                countHeart++;
                countHint++;
                newGame();
            }
            else if(hardGame(x,y)){
                level = 3;
                countHeart++;
                countHint++;
                newGame();
            }
            else if(soundOnOff(x,y)){
                if (isSoundPlaying){
                    Mix_PauseMusic();
                    isSoundPlaying = false;
                    play(question,result,fillArray);
                }
                else {
                    Mix_ResumeMusic();
                    isSoundPlaying = true;
                    play(question,result,fillArray);
                }
            }
            else num = computePos(x, y);
        }
    }
    return num;
}

void newGame() {
    soundWhenClick();
    if (level == 1) {
        cout << "Easy Level New Game" << endl;
        chonDeDe();
    }
    else if (level == 2) {
        cout << "Medium Level New Game" << endl;
        chonDeTrungBinh();
    }
    else if (level == 3) {
        cout << "Hard Level New Game" << endl;
        chonDeKho();
    }
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            question[row][col] = board[row][col];
        }
    }
    if (solveSudoku(board, 0, 0)) {
        for (int row = 0; row < N; row++) {
            for (int col = 0; col < N; col++) {
                result[row][col] = board[row][col];
            }
        }
    }
    else cout << "No answer";
    memset(fillArray,0,sizeof(fillArray));
    play(question, result,fillArray);
}
/********************** Playing ************************/



/****************** Deal with Retire ***********************/
void dealDontWantPlay(){
    soundWhenClick();
    renderTexture(niceTry, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_RenderPresent(renderer);
    Mix_HaltMusic();
    soundWhenFail();
    opinionInEnd();
}
void playAgainInRetire() {
    SDL_Event e;
    bool running = true;

    while (running) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
                exitApp();
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x;
                int y = e.button.y;

                if (isNewGame(x,y)) {
                    countHeart++;
                    countHint++;
                    newGame();
                }
                else if(retire(x,y)){
                    dealDontWantPlay();
                }

                else if (easyGame(x,y)) {
                    level = 1;
                    countHeart++;
                    countHint++;
                    cout << "Easy Level" << endl;
                    newGame();
                }
                else if(mediumGame(x,y)){
                    level = 2;
                    countHeart++;
                    countHint++;
                    cout << "Medium Level" << endl;
                    newGame();
                }
                else if(hardGame(x,y)){
                    level = 3;
                    countHeart++;
                    countHint++;
                    cout << "Hard Level" << endl;
                    newGame();
                }
            }
        }
        SDL_Delay(10);
    }
}
void showAnswer(){
    soundWhenClick();
    showGame( question);
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            if (fillArray[i][j] == 0 and  0 == question[i][j]) fillArray[i][j] = result[i][j];
        }
    }
    fillAnswer(fillArray);
    drawHeart(timesWrongArray[countHeart]);
    drawHint(hintArray[countHint]);
    playAgainInRetire();
}
/****************** Deal with Retire ***********************/


/******************Load and Unload Images***********************/
void load_SDL_and_Images(){
    initSDL(window, renderer);

    mainScreenEasy = loadTexture("SudokuImages/mainScreenEasy1.bmp", renderer);
    mainScreenMedium = loadTexture("SudokuImages/mainScreenMedium1.bmp", renderer);
    mainScreenHard = loadTexture("SudokuImages/mainScreenHard1.bmp", renderer);

    wrong = loadTexture("SudokuImages/wrong.bmp", renderer);
    correct = loadTexture("SudokuImages/correct.bmp", renderer);
    success = loadTexture("SudokuImages/success.bmp", renderer);

    hint1   = loadTexture("SudokuImages/13.bmp", renderer);
    hint2   = loadTexture("SudokuImages/23.bmp", renderer);
    hint3   = loadTexture("SudokuImages/33.bmp", renderer);
    hint0   = loadTexture("SudokuImages/03.bmp", renderer);
    heart   = loadTexture("SudokuImages/heart.bmp", renderer);
    gameover = loadTexture("SudokuImages/gameOver.bmp", renderer);
    congratulation = loadTexture("SudokuImages/congratulation.bmp", renderer);
    niceTry = loadTexture("SudokuImages/niceTry1.bmp", renderer);
    startScreen = loadTexture("SudokuImages/startScreen.bmp", renderer);
    introduction1 = loadTexture("SudokuImages/introduction1.bmp", renderer);
    introduction2 = loadTexture("SudokuImages/introduction2.bmp", renderer);
    tran = loadTexture("SudokuImages/tran.bmp", renderer);
    soundOn = loadTexture("SudokuImages/soundOn.bmp", renderer);
    soundOff = loadTexture("SudokuImages/soundOff.bmp", renderer);
}

void unload_SDL_and_Images(){
    SDL_DestroyTexture(mainScreenEasy);
    SDL_DestroyTexture(mainScreenMedium);
    SDL_DestroyTexture(mainScreenHard);

    SDL_DestroyTexture(correct);
    SDL_DestroyTexture(wrong);
    SDL_DestroyTexture(success);
    SDL_DestroyTexture(heart);
    SDL_DestroyTexture(soundOn);
    SDL_DestroyTexture(soundOff);

    SDL_DestroyTexture(gameover);
    SDL_DestroyTexture(congratulation);
    SDL_DestroyTexture(niceTry);

    SDL_DestroyTexture(startScreen);
    SDL_DestroyTexture(introduction1);
    SDL_DestroyTexture(introduction2);

    SDL_DestroyTexture(hint0);
    SDL_DestroyTexture(hint1);
    SDL_DestroyTexture(hint2);
    SDL_DestroyTexture(hint3);

    SDL_DestroyTexture(number1);
    SDL_DestroyTexture(number1);
    SDL_DestroyTexture(number2);
    SDL_DestroyTexture(number3);
    SDL_DestroyTexture(number4);
    SDL_DestroyTexture(number5);
    SDL_DestroyTexture(number6);
    SDL_DestroyTexture(number7);
    SDL_DestroyTexture(number8);
    SDL_DestroyTexture(number9);

    SDL_FreeSurface(icon);

    quitSDL(window, renderer);
}
/***************************************************/


/******************Load and Unload Sound***********************/
void load_SDL_and_Sound(){
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return;
    }
    successSound = Mix_LoadWAV("Music//success.wav");
    click = Mix_LoadWAV("Music//click.mp3");
    wrongSound = Mix_LoadWAV("Music//wrong1.wav");
    rightSound = Mix_LoadWAV("Music//correct1.wav");
    failSound = Mix_LoadWAV("Music//fail.wav");
    chooseSound = Mix_LoadWAV("Music//choose.wav");
    backgroundMusic1 = Mix_LoadMUS("Music//backgroundMusic1.mp3");
    backgroundMusic2 = Mix_LoadMUS("Music//backgroundMusic2.mp3");
}

void unload_SDL_and_Sound(){
    Mix_FreeChunk(click);
    Mix_FreeChunk(successSound);
    Mix_FreeChunk(wrongSound);
    Mix_FreeChunk(rightSound);
    Mix_FreeChunk(failSound);
    Mix_FreeChunk(chooseSound);
    Mix_FreeMusic(backgroundMusic1);
}
/******************Load and Unload Sound***********************/



/****************** Solve Sudoku ***********************/
bool isSafe(int board[N][N], int row, int col, int num) {
    for (int i = 0; i < N; i++)
        if (board[row][i] == num) return false;

    for (int i = 0; i < N; i++)
        if (board[i][col] == num)
            return false;

    int startRow = row - row % 3;
    int startCol = col - col % 3;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i + startRow][j + startCol] == num)
                return false;

    return true;
}

bool solveSudoku(int board[N][N], int row, int col) {
    if (row == N - 1 and col == N)
        return true;
    if (col == N) {
        row++;
        col = 0;
    }
    if (board[row][col] != 0)
        return solveSudoku(board, row, col + 1);

    for (int num = 1; num <= 9; num++) {
        if (isSafe(board, row, col, num)) {
            board[row][col] = num;

            if (solveSudoku(board, row, col + 1))
                return true;

            board[row][col] = 0;
        }
    }
    return false;
}

bool isSolvedCompletely(int grid[][N]) {
    for (int row = 0; row < N; row++)
        for (int col = 0; col < N; col++)
            if (grid[row][col] == 0)
                return false;
    return true;
}
/****************** Solve Sudoku ***********************/



//*****************************************************
// Các hàm chung về khởi tạo và huỷ SDL
void logSDLError(std::ostream& os, const std::string &msg, bool fatal){
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    //window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
    //   SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);


    //Khi thông thường chạy với môi trường bình thường ở nhà
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);
    //Khi chạy ở máy thực hành WinXP ở trường (máy ảo)
    //renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));
    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}
void quitSDL(SDL_Window* window, SDL_Renderer* renderer){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
void waitUntilKeyPressed(){
    SDL_Event e;
    while (true) {
        if ( SDL_WaitEvent(&e) != 0 &&
             (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren){
    //Khởi tạo là nullptr để tránh lỗi 'dangling pointer'
    SDL_Texture *texture = nullptr;
    //Nạp ảnh từ tên file (với đường dẫn)
    SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
    //Nếu không có lỗi, chuyển đổi về dạng texture and và trả về
    if (loadedImage != nullptr){
        texture = SDL_CreateTextureFromSurface(ren, loadedImage);
        SDL_FreeSurface(loadedImage);
        //Đảm bảo việc chuyển đổi không có lỗi
        if (texture == nullptr){
            logSDLError(std::cout, "CreateTextureFromSurface");
        }
    }
    else {
        logSDLError(std::cout, "LoadBMP");
    }
    return texture;
}
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){
    //Thiết lập hình chữ nhật đích mà chúng ta muốn vẽ ảnh vào trong
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    //Đưa toàn bộ ảnh trong texture vào hình chữ nhật đích
    //(ảnh sẽ co dãn cho khớp với kích cỡ mới)
    SDL_RenderCopy(ren, tex, NULL, &dst);
}
void clearRegion(SDL_Renderer* renderer, int x, int y, int width, int height) {
    SDL_Rect clearRect = { x, y, width, height };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);  // Đặt màu vẽ là màu trắng (255, 255, 255, 255)
    SDL_RenderFillRect(renderer, &clearRect);  // Vẽ hình chữ nhật trắng lên vùng cần xóa
    SDL_RenderPresent(renderer);  // Cập nhật màn hình
}
//**************************************************************

