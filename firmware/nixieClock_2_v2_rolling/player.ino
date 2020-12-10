// Проигрыватель мелодии будильника
// Использует tone() т.е. таймер 2

// Формат: {частота_ноты_1, длительность_ноты_1, ....., частота_ноты_N, длительность_ноты_N}
// Пауза - это нота с частотой 0
// Частота в Гц, длительность в мс
uint16_t song[] = {494,200,0,300,740,200,0,50,494,200,0,50,784,200,0,300,740,200,0,50,659,200,
                    0,50,740,200,0,300,659,200,0,50,740,200,0,50,784,200,0,50,784,200,0,50,
                    740,200,0,50,659,200,0,50,494,200,0,300,740,200,0,50,494,200,0,50,784,200,
                    0,300,740,200,0,50,659,200,0,50,587,200,0,300,659,200,0,50,587,200,0,50,554,
                    200,0,50,554,200,0,50,587,200,0,50,554,200,0,50,494,200};

const uint32_t repeatDelay = 5000;	// задержка между повторами мелодии, мс

const uint8_t songLength = sizeof(song) / sizeof(song[0]);
volatile boolean playing = false;
timerMinim playTimer(1);
volatile uint8_t playCounter = 0;

void play() {
  if (playing) return;
  playCounter = 0;
  playTimer.setInterval(1);
  playTimer.reset();
  playing = true;
}

void noPlay() {
  playing = false;
  noTone(PIEZO);
}

void playerTick() {
  if (playing && playTimer.isReady()) {

    if (playCounter >= songLength) {
      noTone(PIEZO);
      playCounter = 0;
      playTimer.setInterval(repeatDelay);
      playTimer.reset();
    } else {
    
      if(song[playCounter] > 0) {
        tone(PIEZO, song[playCounter]);
      } else {
        noTone(PIEZO);
      }
      
      playTimer.setInterval(song[playCounter+1]);
      playTimer.reset();
  
      playCounter+=2;
    }
  }
}
