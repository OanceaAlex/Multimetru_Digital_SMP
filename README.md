# Multimetru_SMP
Proiect Multimetru Digital pentru materia SMP anul 3
Autor: Oancea Alexandru - Nicolae 332AB

Codul programului este scris in "C++"/Arduino Code si este destinat incarcarii pe o placuta Arduino UNO / NANO

Programul are 5 moduri de functionare (indexate de la 0 pana la 4), fiecare destinat masurarii unei marimi fizice:
- Tensiune (Voltage)
- Rezistenta (Resistance)
- Capacitate (Capacitance)
- Inductanta (Inductance)
- Intensitate curent (Amperage)

Pentru a comuta intre modurile de functionare se foloseste butonul "mid_but".

Modul de masurare a rezistentei are trei scale de masurare distincte, acestea pot fi comutate folosund butonul "top_but"

Butonul de jos "bot_but" nu a fost folosit din lipsa de timp si pentru a evita aparitia de boi bug-uri :)

Modul de capacitate are doua scale de masurare care sunt comutate automat dupa nevoie.

Rezultatul afisat in modul de masurare a curentului depinde de tensiunea de alimentare a circuitului, insa eroarea ar trebui
sa aiba doar un efect de offset, diferenta intre valoarea cand multimetrul este deconectat de la curentul care trebuie masurat
si atunci cand el se conecteaza ar trebui sa reprezinte corect valoarea reala a intensitatii curentului.

Pentru afisarea valorilor masurate am folosit un LCD display 16x2 cu comunicare folosind protocolul I2C
