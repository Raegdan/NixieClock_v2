// динамическая индикация по прерыванию канала B таймера 0

ISR(TIMER0_COMPB_vect) {
    indiCounter[curIndi]++;             // счётчик индикатора
    if (indiCounter[curIndi] >= indiDimm[curIndi])  // если достигли порога диммирования
      setPin(opts[curIndi], 0);         // выключить текущий индикатор
  
    if (indiCounter[curIndi] > 25) {    // достигли порога в 25 единиц
      indiCounter[curIndi] = 0;         // сброс счетчика лампы
      if (++curIndi >= 4) curIndi = 0;  // смена лампы закольцованная
  
      // отправить цифру из массива indiDigits согласно типу лампы
      if (indiDimm[curIndi] > 0) {
        byte thisDig = digitMask[indiDigits[curIndi]];
        setPin(DECODER3, bitRead(thisDig, 0));
        setPin(DECODER1, bitRead(thisDig, 1));
        setPin(DECODER0, bitRead(thisDig, 2));
        setPin(DECODER2, bitRead(thisDig, 3));
        setPin(opts[curIndi], anodeStates[curIndi]);    // включить анод на текущую лампу
      }
    }
}