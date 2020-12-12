// Проигрыватель мелодии будильника
// Использует таймер 2

// Формат: {частота_ноты_1, длительность_ноты_1, ....., частота_ноты_N, длительность_ноты_N}
// Пауза - это нота с частотой 0
// Частота в Гц, длительность в мс
#if (ALARM_TYPE == 1)
/*uint16_t song[] = {494,200,0,300,740,200,0,50,494,200,0,50,784,200,0,300,740,200,0,50,659,200,
                    0,50,740,200,0,300,659,200,0,50,740,200,0,50,784,200,0,50,784,200,0,50,
                    740,200,0,50,659,200,0,50,494,200,0,300,740,200,0,50,494,200,0,50,784,200,
                    0,300,740,200,0,50,659,200,0,50,587,200,0,300,659,200,0,50,587,200,0,50,554,
                    200,0,50,554,200,0,50,587,200,0,50,554,200,0,50,494,200};

const uint8_t songLength = sizeof(song) / sizeof(song[0]);
*/
uint8_t songLength = 0;
uint16_t song[110];

const uint16_t repeatDelay = 5000;	// задержка между повторами мелодии, мс

volatile boolean playing = false;
timerMinim playTimer(1);
volatile uint8_t playCounter = 0;

void initPlayer() {
  if (songLength > 0) return;
  EEPROM.get(100,songLength);
  EEPROM.get(101,song);
}

void play() {
  if (playing) return;
  initPlayer();
  playCounter = 0;
  playTimer.setInterval(1);
  playTimer.reset();
  playing = true;
}

void noPlay() {
  playing = false;
  beep(0);
}

void playerTick() {
  if (playing && playTimer.isReady()) {

    if (playCounter >= songLength) {
      beep(0);
      playCounter = 0;
      playTimer.setInterval(repeatDelay);
      playTimer.reset();
    } else {
    
      if(song[playCounter] > 0) {
        beep(1, song[playCounter]);
      } else {
        beep(0);
      }
      
      playTimer.setInterval(song[playCounter+1]);
      playTimer.reset();
  
      playCounter+=2;
    }
  }
}
#endif
