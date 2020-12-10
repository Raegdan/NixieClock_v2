timerMinim playTimer(1);
volatile uint8_t playCounter = 0;
uint16_t song[] = {494,200,0,300,740,200,0,50,494,200,0,50,784,200,0,300,740,200,0,50,659,200,
                    0,50,740,200,0,300,659,200,0,50,740,200,0,50,784,200,0,50,784,200,0,50,
                    740,200,0,50,659,200,0,50,494,200,0,300,740,200,0,50,494,200,0,50,784,200,
                    0,300,740,200,0,50,659,200,0,50,587,200,0,300,659,200,0,50,587,200,0,50,554,
                    200,0,50,554,200,0,50,587,200,0,50,554,200,0,50,494,200};

const uint8_t songLength = sizeof(song) / sizeof(song[0]);
const uint32_t repeatDelay = 5000;

volatile boolean playing = false;

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
