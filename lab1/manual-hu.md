# Oktatási célok

A gyakorlat során input validációs kódot kell készíteni egy egyedi fájlformátumhoz. 
A gyakorlat célja, hogy bemutassa, milyen fontos meggyőződni arról, hogy az alkalmazásnak adott bemenetek nem tudnak meghibásodást okozni. 
A mérés során egy egyedi fájlformátum, a CrySyS Image File Format (CIFF), specifikációját kapja kézhez, valamint a megjelení­tését naí­van (ellenőrzések nélkül) implementáló alkalmazást. 
A feladata az, hogy egészítse ki a kódot a szükséges ellenőrzések implementálásával úgy, hogy hibás bemeneteket ne jelení­tsen meg az alkalmazás. 
A gyakorlat elvégzésével képessé válik arra, hogy azonosítsa egy specifikációban az implicit és explicit előfeltételezéseket a bemenő adatokkal szemben, valamint képessé válik olyan kód implementálására, ami ellenőrzi az előfeltételezések fennállását bármilyen bemenet esetén.



  

# Háttéranyag

## Adatok feldolgozása    

Ahhoz, hogy kevesebb bug legyen az alkalmazásainkban és a kompromittálások súlyossága csökkenjen, figyelembe kell venni az alkalmazások *támadási felületét*.  
Ez a fogalom azokat a kódrészeket jelenti, amelyek azért felelősek, hogy az alkalmazás hogyan lép interakcióba a felhasználókkal és más folyamatokkal, vagyis itt dől el, hogy milyen adatok feldolgozása fog megtörténni.  
A támadási felületet az alábbiak alkotják:

- Az alkalmazás számára értékes és/vagy szenzitív adat és az ezt védő kódszegmensek, valamint  
- azok a végrehajtási ágak, amiken keresztül adat és/vagy parancs érkezik vagy távozik, és az ezeket védő kódrészletek.  

A támadási felület azonosítása az első lépés ahhoz, hogy input validációt valósítsunk meg az alkalmazásokban.  
Alapvetőnek kell lennie, hogy a felhasználóktól érkező bármely bemenet akár egy a támadótól is származhat, így minden értéket megbízhatatlanként kell először kezelni.  
Egy megbízhatatlan bemenet ellenőrzése három nagyobb lépésből áll:

1. **Normalizálás / kanonikalizálás**: A bemenet átalakítása annak legegyszerűbb és elvárt formájára. Például, amikor a bemeneti karaktersorozatokat átkonvertáljuk egy előre meghatározott kódolásra (pl. Unicode), akkor normalizáljuk ezeket a bemeneteket, mivel a karaktersorozat bináris reprezentációja megváltozik (az eltárolt bitek megváltoznak, a szemantika viszont megmarad). Egy másik példa az XML dokumentumok esete, ahol az üres tageket kétféleképpen is leírhatjuk: `<tag/>` vagy `<tag></tag>`. Amikor normalizáljuk a bemenetet, hogy csak az egyik leírás szerepelhessen, azzal egy átalakítást végzünk egy ekvivalens formára.

2. **Szűrés**: Bizonyos elemek törlése megadott kritérium alapján. Például, a `<` és `>` karakterek törlése webes formok bemeneteiből egy olyan szűrési lépés, amivel bizonyos webes támadásokat (pl. cross-site scripting) próbálunk kivédeni. Egy másik példa pedig az lehet, amikor dátumok feldolgozása során kiszűrjük a nem lehetséges értékeket, például azokat az eseteket, amikor a hónap értéke nagyobb mint 12. A szűrést mindig normalizálás / kanonikalizálás után végezzük el!

3. **Validálás**: Annak ellenőrzése, hogy az adat szemantikailag ésszerű-e, vagyis hogy a tartalma olyan jelentést hordoz-e, aminek van értelme az adott kontextusban. Például, ha a bemenet egy születési dátum, akkor a validálás során érdemes ellenőrizni, hogy az nem régebbi-e 100-120 évnél. A pontos határérték meghatározás alkalmazásfüggő lehet, vagy a programozó felelőssége. Ehhez hasonlóan azt is érdemes lehet vizsgálni, hogy nem újabb-e a dátum az aktuális dátumnál, vagy egy olyan időpontnál, ami után születettek nem használhatják az alkalmazást. A validációt meg kell előznie a normalizálásnak és a szűrésnek is!


Egy robusztus és biztonságos kód írásához mind a három lépés elvégzése szükséges.

## Python property-k és dekorátorok

Jól karbantartható kód írásához érdemes használni az adott programozási nyelv adta legtöbb lehetőséget vagy a közösség által megalkotott konvenciókat. Ezek követése elősegíti, hogy több fejlesztő tudjon együttműködni és közösen egy kódbázison dolgozni.

A Python nyelv alapvetően nem rendelkezik olyan hozzáférés módosító kulcsszavakkal, mint amilyen a `private` vagy a `protected`, ezért a Python programozók csak elnevezési konvenciót használnak: ha egy attribútum neve aláhúzással kezdődik, pl. `class._x`, akkor az attribútumot privátként kezelik, egyébként publikusként.  
A konvenció könnyebb betartásához érdemes néhány programozási mintát követni, hogy kisebb valószínűséggel maradjon hiba a kódban. Az adat beágyazás eléréséhez használhatunk például getter és setter metódusokat, ahogy azt az alábbi példa is mutatja.

```python
class Example:
    def __init__(self, v):
        """
        Constructor
        """
        self._value = v
        
    def get_value(self):
        """
        Getter method
        """
        return self._value
        
    def set_value(self, v):
        """
        Setter method
        """
        
        # ...
        # input validation
        # ...
        
        self._value = v
```

Ennek a megoldásnak az a hátránya, hogy a beágyazott adatokkal végzett számításokat nehéz olvasni.  
Vegyük például azt az esetet, amikor a `_value` attribútum egész számokat tartalmaz.  
Ahhoz, hogy két `_value`-t összeadjuk az `Example` osztály két példányából, a következő kódra van szükség:

```python
a = Example(1)
b = Example(2)
result = a.get_value() + b.get_value()
```

Minél összetettebb a számítás, annál nehezebben olvasható a kód.  
Az osztály akár implementálhatna egy `add(self, Example)` metódust is, hogy jobban megfeleljen az objektum-orientált programozás szabályainak, de ez a megoldás sem segítene az olvashatóságon.

A probléma megoldását a _property-k_ adják.  
Ezzel a konstrukcióval a programozók tarthatják az elnevezési konvenciókat és adat beágyazást is implementálhatnak.  
Property-k létrehozásához a speciális `property()` függvényt kell használni, ami argumentumként három függvényt (egy getter, egy setter és egy deleter) és egy dokumentációs sztringet vár.  
A property-t aztán az attribútum helyett lehet használni a kódban.

```python
class Example:
    def __init__(self, v):
        """
        Constructor
        """
        self._value = v
        
    def get_value(self):
        """
        Getter method
        """
        return self._value
        
    def set_value(self, v):
        """
        Setter method
        """
        
        # ...
        # input validation
        # ...
        
        self._value = v
        
    value = property(get_value, set_value, None, "")
 

a = Example(1)
b = Example(2)
result = a.value + b.value
```

Amikor a property-t olvassuk, a paraméterként megadott getter metódus fut le. Amikor a property-t írjuk, akkor a megadott setter metódus hívódik meg.

A `property()` függvény egy ún. *dekorátor* függvény: olyan függvény, ami paraméterként egy másik függvényt vár, módosítja annak végrehajtását és visszatér vele.  
A dekorátor függvényeket közvetlenül meghívhatjuk, ahogy azt az előző példában láthattuk.  
Azonban a speciális `@` szimbólum használatával úgy dekorálhatunk függvényeket, hogy nem kell explicit meghívni a dekorátor függvényt.  
Az előző példát ennek megfelelően az alábbiként is írhatjuk:

```python
class Example:
    def __init__(self, v):
        """
        Constructor
        """
        self._value = v
        
    @property
    def value(self):
        """
        Getter
        """
        return self._value
        
    @value.setter
    def value(self, v):
        """
        Setter
        """
        
        # ...
        # input validation
        # ...
        
        self._value = v
```

A `value(self)` metódust úgy dekoráltuk, hogy az az ugyanazon nevű property getter metódusa legyen. 
Az osztály tartalmazza továbbá a `value(self, t)` metódust (vegyük észre az operátor túlterhelést!), amit a `value` property setterévé dekoráltunk. 
A property felhasználása nem változik, az előzőeknek megfelelően lehet használni.

A Python három beépített dekorátort ismer, amikor a `@` karakterrel használhatunk. 
Ezek a következőek:

- `property` - property-k létrehozása, ahogy az előbbiekben tárgyaltuk,
- `staticmethod` - statikus metódus létrehozása, ami nem fér hozzá semmilyen `self` változóhoz, és
- `classmethod` - osztálymetódus létrehozása, ami úgy módosítja a metódust, hogy az első paraméterként nem a `self` objektumot kapja, hanem az osztályt.

Saját dekorátorokat is létrehozhatunk, ezek tárgyalása azonban túlmutat ezen a laboratóriumi gyakorlaton. 
További információ a [https://realpython.com/primer-on-python-decorators/](https://realpython.com/primer-on-python-decorators/) oldalon található.

## CrySyS Image File Format

A CrySyS Image File Format (CIFF) egy egyedi, tömörítetlen képformátum. Oktatási céllal jött létre, laborok és házi feladatok megvalósításához.
Egyedisége miatt nem létezik olyan szabadon hozzáférhető könyvtár vagy modul, ami a feldolgozását implementálná. 
A fájlformátum áttekintő ábráját az alábbi ábra mutatja.

![CIFF formátum](figures/ciff_hu.png "CIFF formátum")

## CIFF specifikáció

A CIFF specifikációnak megfelelő fájlok egy fejléccel kezdődnek. A fejléc az alábbi részekből tevődik össze:

- **Magic:** 4 karakter, amik együttesen a "CIFF" szócskát adják.
- **Fejléc mérete:** Egy 8 bájt hosszú egész szám, aminek az értéke megadja a fejléc méretét (minden mezőt figyelembe véve), vagyis a fájl első `fejléc mérete` számú bájtjai adják az egész fejlécet.
- **Tartalom mérete:** Egy 8 bájt hosszú egész szám, aminek az értéke megadja a fájl végén található pixelek méretét. Az értékének egyeznie kell a `szélesség * magasság * 3` értékkel.
- **Szélesség:** Egy 8 bájt hosszú egész szám, aminek az értéke a kép szélessége. Az értéke lehet 0, de ebben az esetben nem lehetnek pixelek a fájlban.
- **Magasság:** Egy 8 bájt hosszú egész szám, aminek az értéke a kép magassága. Az értéke lehet 0, de ebben az esetben nem lehetnek pixelek a fájlban.
- **Képaláírás:** Változó hosszú ASCII karaktersorozat, ami `\n`-nel végződik és a képaláírást adja meg. Mivel a `\n` speciális karakter a fájl formátum számára, a képaláírás nem tartalmazhat ilyen karaktert.
- **Címkék:** Változó darab számú, változó hosszúságú ASCII karaktersorozat, amiket `\0` választ el. A címkék nem lehetnek többsorosak. Az utolsó címkét is `\0`-nak kell követnie.

A fejlécet a képet alkotó pixelek követik RGB formátumban, minden komponens egy bájtot tesz ki. 
A pixelek pontosan `tartalom mérete` bájtot kell, hogy kitegyenek.


A laboratóriumi gyakorlathoz egy olyan naív Python alkalmazást fog kapni, ami képes a specifikációnak megfelelő CIFF képeket megjeleníteni. 
Az alkalmazás két fájlból áll, `view.py` és `ciff.py`. 
Az első implementálja a grafikus felhasználói felületet, a második pedig a CIFF képeket reprezentáló osztályt. 

A feladata az, hogy a második fájlban található osztályban implementálja az input validációhoz szükséges ellenőrzéseket. 

## Kis segítség az osztály kódjának megértéséhez

- Az osztály több property-vel is rendelkezik, külön-külön a fejléc minden részéhez és a pixelek listájához is. A property-khez tartoznak getter és setter metódusok is.
- Az `is_valid` property jelzi az alkalmazás számára, hogy a `CIFF` osztály egy példánya egy érvényes CIFF kép adatait tartalmazza-e. A property getter metódusának boolean értéket kell visszaadnia.
- Az osztály tartalmaz egy statikus feldolgozó metódust, amit `@staticmethod` dekorátorral láttunk el, ez a `CIFF.parse_ciff_file()`. Ez a metódus a CIFF fájl beolvasásakor hívódik meg és a `CIFF` osztály egy példányát kell visszaadnia. A metódus a `struct` Python modult használja fel, hogy a megadott formátumnak megfelelően értelmezze a fájlból beolvasott bájtok egy sorozatát. A modul dokumentációja [itt érhető el](https://docs.python.org/3/library/struct.html). A formátumot meghatározó karaktereket meg kell ismernie a gyakorlat előtt!

## Python kód debuggolása

Python alkalmazások debuggolására két alapvető megközelítés létezik.  
Az első megközelítés a fejlesztőkörnyezetbe, pl. IDLE, VSCode, épített funkciókra épül.  
Az IDLE debuggoláshoz használható funkcióinak egy jó áttekintése [itt](https://www.cs.uky.edu/~keen/help/debug-tutorial/debug.html) érhető el.  
A laboratóriumi gyakorlaton való részvételhez kötelező ennek az oldalnak az átolvasása!  
VSCode környezetben pedig [ez](https://code.visualstudio.com/docs/python/debugging) a leírás segíthet a hibakeresésben.

A második megközelítés egy kifejezetten debuggolásra készített Python modulra épül és olyan esetekben alkalmazható, amikor nincs grafikus felhasználói felület vagy fejlesztőkörnyezet.  
A modul neve `pdb`, Python Debugger, amit használat előtt importálni kell a kódba.  
Importálás után breakpointot a `pdb.set_trace()` függvénnyel helyezhetünk el a kódba.  
A függvény megállítja a végrehajtást meghívásakor és lehetőséget nyújt a fejlesztőnek arra, hogy megvizsgáljon változókat, scope-on belüli függvényeket és metódusokat hívjon meg, valamint egyszerű Python utasításokat adjon ki.  
A modul különböző parancsokat is implementál, ezek közül a legismertebbek a következőek:

- `p` - print, változók és egyéb számítási eredmények kiíratása,
- `s` - step into, következő utasításként végrehajtandó függvénybe/metódusba lépés,
- `n` - next, következő sor végrehajtása,
- `u` - until, végrehajtás, amíg egy olyan sorhoz nem érünk, aminek a sorszáma nagyobb (kifejezetten ciklusok átlépéséhez hasznos), és 
- `c` - continue, következő breakpointig vagy kilépésig folytatja a végrehajtást.


# Környezet felállítása
## VM használata

A VirtualBox alkalmazást használva importáljuk a kiadott OVA fájlt, majd indítsuk el a VM-et. A kiadott VM-et használva semmilyen további előkészület nem szükséges.

## Otthoni környezet használata
Amennyiben nem a kiadott virtuális gépet (VM) használjuk, kövessük a mellékelt Python-útmutatót a virtuális környezet beállításához. Ezt követően telepítsük és ellenőrizzük az alábbi függőségeket:

- Python 3.7 vagy újabb verzió (rendszerszinten telepítve)
- python-tk 3.13 – stabil verzió: 3.13.1 (rendszerszinten telepítve)
- pillow 11.1.0 (virtuális környezetben telepítve)


## Kiinduló kód beállítása

Töltsük le a kiindulási kódot az oldalról: https://software.crysys.hu/it-security/input-validation



# Feladatok

A labor gyakorlat elején először értelmezzük a specifikációt és írjuk össze, hogy milyen ellenőrzéseket kell implementálni!

- Minden beolvasott bájt után az implementációnak ellenőriznie kell, hogy elérte-e a fájl végét (end of file, EOF). Ehhez meg kell nézni, hogy az olvasás üres sztringet adott-e vissza.
- **Magic**: A karaktereknek a `CIFF` szócskát kell kiadniuk. A naiv implementáció jelenleg egyetlen művelettel dekódolja a 4 karaktert, de ha olyan teszt vektort kapna, ami csak 3 karakterből áll, összeomlana.
- **Fejléc mérete**: $\in [38; 2^{64}-1)$, vagyis legalább 38, maximum értéknek meg egy választott kisebb érték is lehetne, ha korlátozni szeretnénk, de legyen most a megadott érték. A $38$-at úgy kapjuk meg, hogy kiszámoljuk a lehető legrövidebb, még érvényes fejléc méretét (üres képaláírás, nincsenek címkék). A $2^{64}-1$ a legnagyobb szám, amit a *fejléc mérete* mezőben találhatunk. A naiv implementáció előjeles egészként olvassa be ezt a mezőt (a `struct` modul `q` formátum stringjét használja `Q` helyett), emiatt a $2^{64}-1$-t $-1$-nek értelmezi.
- **Tartalom mérete**: $\in [0; 2^{64}-1)$, mivel elképzelhető, hogy a fájlban egyáltalán nincsenek pixelek, de az is lehet, hogy a lehető legtöbb pixelt tartalmazza a fájl. A mező értékének meg kell egyeznie a `szélesség*magasság*3` értékkel. A naiv implementáció előjeles számként olvassa be (`struct` formátum string).
- **Szélesség**: $\in [0; 2^{64}-1)$, ugyanazok a megfontolások érvényesek, mint a *tartalom mérete* esetén. A naiv implementáció előjeles számként olvassa be (`struct` formátum string).
- **Magasság**: $\in [0; 2^{64}-1)$, ugyanazok a megfontolások érvényesek, mint a *tartalom mérete* esetén. A naiv implementáció előjeles számként olvassa be (`struct` formátum string).
- **Képaláírás**: Csak ASCII karaktereket tartalmazhat. A naiv implementáció megpróbálja így dekódolni a beolvasott bájtot, de a kódból hiányzik a hibakezelés (`try-except` arra az esetre, ha a bájtot nem lehet ASCII karakterként dekódolni). Ahhoz, hogy a képaláírás semmiképp ne tartalmazzon `\n` karaktert, az első ilyen karakter beolvasása után úgy kell kezelni a fájlt, hogy elértük a képaláírás végét (a naiv implementáció ezt teszi).
- **Címkék**: Csak ASCII karaktereket tartalmazhat, a beolvasásnál itt is hiányzik a hibakezelés. A beolvasott karaktereket egyenként ellenőrizni kell, hogy egyik se legyen `\n`. A naiv implementáció úgy olvassa be a címkéket, hogy a `\0`-kat is beolvassa a címkék végére, de ezeket nem jeleníti meg a grafikus felületen. Ellenőriznünk kell, hogy a legutolsó címke végén is szerepel-e a `\0`!
- **Pixelek**: Pontosan annyi pixelt kell tartalmaznia a fájlnak, amennyit a *tartalom mérete* mező mond. Ezt könnyen implementálhatjuk úgy, hogy rendszeresen ellenőrizzük, hogy elértük-e az EOF-ot: a fájl érvénytelen, ha hamarabb elérjük a fájl végét, mint kéne, vagy ha nincs EOF a megadott darabnyi pixel után.

Példakód a magic karakterek ellenőrzéséhez:

```python
# read the magic bytes
magic = ciff_file.read(4)
# read may not return the requested number of bytes
# TODO: magic must contain 4 bytes. If not, raise Exception
if len(magic) != 4:
    raise Exception("Invalid magic: length")
bytes_read += 4
# decode the bytes as 4 characters
try:
    new_ciff.magic = magic.decode('ascii')
except Exception as e:
    raise Exception("Invalid magic: non-ASCII")
# TODO: the magic must be "CIFF". If not, raise Exception
if new_ciff.magic != "CIFF":
    raise Exception("Invalid magic: value")
```


A labor teljesítéséhez két helyen lehet módosítani a `ciff.py` fájlt:

1. A `CIFF.is_valid` property-hez tartozó metódusban lehet implementálni a validációs ellenőrzéseket a beolvasást követően, vagy
2. A `CIFF.parse_ciff_file()` metódusban a beolvasással kapcsolatos hiányzó ellenőrzéseket és format string hibákat mindenképpen javítani kell, de itt meg lehet valósítani a teljes validációt is.

A naiv implementáció `CIFF.parse_ciff_file()` metódusában található egy kikommentezett `try`-`except` blokk.  
Ez a blokk bármilyen kivétel keletkezése esetén beállítja a `CIFF.is_valid` property-t `false` értékre.  
Amennyiben kikommentezi ezt a `try`-`except` blokkot, elég csak kivételeket dobnia a megfelelő helyeken, és a kód automatikusan érvénytelennek fogja minősíteni a bemenetet.  

A munkát segítendő, a `CIFF.parse_ciff_file()` metódusban több helyen is `TODO:` kommentek találhatóak, amikben röviden le van írva a megvalósítandó validáció.  
A teszteléshez használható példa fájlok a `test-vectors` mappában találhatóak.

Az elkészült implementációról leadandó beszámolót a `moodle_submission.py` szkript készíti el.
A szkript meghívja a `CIFF.parse_ciff_file()` metódust minden tesztvektorra, és a `CIFF.is_valid` property értéke alapján kiírja, hogy az adott fájlt érvényes vagy érvénytelen bemenetnek érzékelte-e a kód.  
Amennyiben egy bemenetre összeomlana a kód, azt is jelzi a kimeneten.  
A munka végeztével futtassa a `moodle_submission.py` szkriptet, és a kimenet alapján töltse ki a Moodle kérdéssort!
Ezt a szkriptet a `view.py` által létrehozott GUI felületen is le lehet futtatni.

