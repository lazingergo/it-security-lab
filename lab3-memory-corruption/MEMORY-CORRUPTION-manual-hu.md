# Memory Corruption

2026-03-24

## Bevezetés

A mérés célja, hogy a hallgatók megismerkedjenek a szoftver implementációs hibák néhány fontosabb kategóriájával. A hibák felismerésén felül a mérés során a hallgatók néhány fontosabb segédeszköz használatát is elsajátíthatják, melyek segítségével implementációs hibák kihasználására is lehetőség nyílik. Ezután, utolsó lépésként, a hallgatók feladata, hogy különböző védekezéseket próbáljanak ki a korábban elkészített támadásokkal szemben.

## Elméleti összefoglalóREADME

A méréshez szükséges anyag elméleti részei a Szoftverbiztonság tárgy Memory Corruption diáiban találhatóak. Az ott található alapokon mellett az alábbi részletek segíthetnek a mérés elvégzésében.

## Fordítás

A mérés során sokszor szükség van az elemzett alkalmazást újrafordítani a megfelelő kapcsolókkal. Ezt hatékonyan make segítségével lehet megoldani. Minden feladathoz mellékelve található egy makefile, ami az összes szükséges konfigurációt tartalmazza a munka megkönnyítéséhez. A mérés elején javasolt a makefile átnézése!

## Hatékony paraméterátadás

A mérés során elemzett alkalmazások a legtöbb esetben command line paramétereket dolgoznak fel. Egy támadás során gyakran előfordul, hogy nagy számú egyforma karakter átadására van szükség. Ezt hatékonyan meg lehet tenni, ha a bash képességeit valamelyik script nyelvvel kombináljuk. Egy egyszerű példa python használatával:

```bash
./app "$(python3 -c 'print("A"*4 + "\x01\x02\x03\x04")')"
```

Nyers byte-ok átadása közvetlenül bash-ben:

```bash
./app $'AAAA\x01\x02\x03\x04'
```

## Hasznos GDB parancsok

A mérés során az alkalmazások vizsgálatára gdb használata javasolt. Az ismertebb utasításokon túl a következő parancsok lehetnek még hasznosak:

- `b *<address>`: Breakpoint elhelyezése egy megadott címre.
- `print foo`: A `foo` függvény címe a memóriában.
- `print 'malloc@plt'`: A `malloc` függvény címe a plt táblában.^1 A külső library-kből hívott függvények indirekten kerülnek meghívásra a `.plt` táblán keresztül.
- `disas`: Egy adott függvény, cím disassembly-je.
- `x/[num]x <address>`: `num*8` byte hosszan kiírja egy memóriaterület tartalmát.

## Függvény hívás folyamata

A függvény hívás során a stack kezelés egy része a hívó függvényben, egy része pedig a hívott függvényben valósul meg.

### A hívó függvény feladata

Mielőtt a végrehajtás átugorhat egy függvényre, a hívó függvénynek két feladata van. Első lépésként a hívott függvény futásához szükséges paramétereket kell elhelyezni a stack-en, amiknek a sorrendje a hívási konvenciótól függhet. Második lépésként pedig egy visszatérési cím elhelyezése a feladat. Ez alapján tudja a hívott függvény, hogy melyik címen kell majd folytatni a végrehajtást a futás után.

### A hívott függvény feladata

A hívott függvény folytatja tovább a stack frame felépítését. A hívó előkészítése után az EBP regiszter elmentése következik. Ezután utolsó lépésként már csak a lokális változóknak szükséges terület lefoglalása szükséges, majd indulhat a függvény tényleges működésének a végrehajtása.

Ezek alapján az előkészített stack a következő képen néz ki:

## Feladatok

### 1. Buffer overflow bevezetés

Az első feladatban egy egyszerű buffer overflow megvalósítása a cél. A sérülékeny alkalmazás a `task-1` mappában található. A támadás célja, hogy az alkalmazás végrehajtását eltérítve a `not_called` függvény is lefusson.

Lépések:

1. Nézze át a makefile-t! Keresse meg, hogy a make parancs kiadása esetén melyik utasítás fog végrehajtódni!
2. A forráskódot megvizsgálva keresse meg a sérülékenységet az alkalmazásban!
3. Fordítsa le az alkalmazást: `make`
4. GDB segítségével elemezze az alkalmazás működését futásidőben!
5. Rajzolja fel a stack-et a támadás előtti állapotban, és közvetlenül utána!
6. Adja meg, hogy milyen támadó input esetén érhető el a kitűzött cél!
7. Javasoljon megoldási javaslatot a problémára a következő szinteken: forráskód módosítás, fordítás, operációs rendszer!
8. Vizsgálja meg, hogy ha a stack smashing protection engedélyezésével fordítja le az alkalmazást (`make withSSP`), akkor véd-e a támadás ellen! Magyarázza meg az eredményt!

### 2. Buffer overflow paraméterekkel

Ebben a feladatban egy paraméter átadással kiegészített buffer overflow támadás megvalósítása a cél. A sérülékeny alkalmazás a `task-2` mappában található. A támadás célja, hogy a sérülékenységet kihasználva hívja meg a `now_called` függvényt, miközben egy valódi függvényhívást szimulálva elhelyezi a stack-en a megfelelő paramétert is.

Lépések:

1. Fordítsa le az alkalmazást: `make`
2. GDB segítségével elemezze az alkalmazás működését futásidőben!
3. Rajzolja fel a stack-et a támadás előtti állapotban, és közvetlenül utána!
4. Adja meg, hogy milyen támadó input esetén érhető el a kitűzött cél!
5. Vizsgálja meg, hogy ha ASLR engedélyezésével fordítja le az alkalmazást (`make withASLR` és `make withASLRwithPIE`), akkor véd-e a támadás ellen! Magyarázza meg az eredményt!

### 3. Return to LibC

A harmadik feladatban egy Return to LibC megvalósítása a cél. A sérülékeny alkalmazás a `task-3` mappában található. Amennyiben egy alkalmazás valamilyen library-t használ, akkor nem csak azokra a függvényekre tud egy támadó ráugrani, amelyeket a programozók írnak, hanem a betöltött library-kben található összes függvényre.

Ebből kifolyólag egy támadás során bármelyik függvény a támadó segítségére lehet, tipikus esetben a LibC library-ből. A támadás célja, hogy az alkalmazás végrehajtását eltérítve a `system` függvény lefusson a megfelelő paraméterrel.

Lépések:

1. Fordítsa le az alkalmazást: `make`
2. GDB segítségével elemezze az alkalmazás működését futásidőben!
3. A LibC library dokumentációja alapján határozza meg, hogy milyen paraméter megadása szükséges a cél eléréséhez.
4. Rajzolja fel a stack-et a támadás előtti állapotban, és közvetlenül utána!
5. Adja meg, hogy milyen támadó input esetén érhető el a kitűzött cél!
6. Vizsgálja meg, hogy ha az NX bit engedélyezésével fordítja le az alkalmazást (`make withNX`), akkor véd-e a támadás ellen! Magyarázza meg az eredményt!

### 4. ROP

A negyedik feladatban egy ROP támadás megvalósítása a cél. A sérülékeny alkalmazás a `task-4` mappában található. Egy ROP támadás során a binárisban található kis kódrészletek (gadget) újrafelhasználásából áll össze egy támadás. Az első lépés, hogy a ROPgadget python script segítségével vizsgálja meg, hogy milyen gadgetek állnak rendelkezésre. Egyáltalán nem biztos, hogy elérhetőek egy elképzelt legegyszerűbb megoldáshoz szükséges gadgetek, itt szükség lehet egy kis kreativitásra. Ezek megfelelő kombinálásával érje el, hogy ismét egy bash terminál hozzáférést lehessen szerezni! Egy ROP támadás esetén a bemenet jelentősen hosszabb tud lenni a korábbi esetekhez képest, így a mappában található `exploit.py` script használata javasolt. Ez a script tartalmaz több segítséget, hogy nagyjából milyen gadget-ek megkeresése lehet célravezető, valamint mintaként is szolgál, hogy hogy lehet összeállítani a támadást.

Lépések:

1. A forráskódot megvizsgálva keresse meg a sérülékenységet és a lehetséges támadást az alkalmazásban!
2. Fordítsa le az alkalmazást: `make`
3. GDB segítségével elemezze az alkalmazás működését futásidőben!
4. Rajzolja fel a stack-et a támadás előtti állapotban, és közvetlenül utána!
5. ROPgadget segítségével keresse meg az elérhető gadgeteket!
6. Az `exploit.py` segítségével állítsa össze a támadást!
7. Vizsgálja meg, hogy ha a stack smashing protection engedélyezésével fordítja le az alkalmazást (`make withSSP`), akkor véd-e a támadás ellen! Magyarázza meg az eredményt!
