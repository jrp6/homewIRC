Tietokoneverkot-projektin dokumentaatio: HomewIRC-keskusteluohjelma
===================================================================

Ohjelman toteutus ja rakenne
----------------------------
Ohjelma on toteutettu käyttäen C:tä ja libevent-kirjastoa. Ohjelma on jaettu kolmeen "moduuliin", jotka ovat omissa
kansioissaan: `client`, `server` ja `common`. Nimiensä mukaisesti `client` sisältää asiakasohjelman, `server` palvelimen
ja `common` sisältää niiden jakamaa yhteistä koodia. Olen pääosin tyytyväinen tekemiini ratkaisuihini, rakenteisiin
ja arkkitehtuuriin, mutta monet osat koodista ovat huonolaatuisia esimerkiksi toisteisuuden vuoksi koska jouduin
priorisoimaan kehitysnopeuden koodin laadun yli kiireessä. Myös kommentointi on kauttaaltaan auttamattoman puutteellista.

`common`-moduulin tiedostot ja niiden tehtävät:

* `close_on_error_eof_cb.c, .h`: Callback-funktio joka vapauttaa buffereventin, ja sen myötä sulkee yhteyden, kun tulee
error- tai end of file -tapahtuma.
* `defaults.h`: Määrittelee oletuksena käytettävän portin 6667, sekä tulisiko asiakkaan tai palvelimen tulostaa debug-tulosteet.
* `hostmask_util.c, .h`: Sisältää apufunktioita hostmaskien sekä nimimerkkien käsittelyyn
* `message.c, .h`: message-tietorakenne joka edustaa viestejä ohjelman sisällä, sekä funktioita mm. tietorakenteen muuntamiseksi
merkkijonoksi ja toisin päin.
* `logMessage.c, .h`: apufunktio joka tulostaa `struct message`n `stdout`iin.
* `replyPing.c, .h`: vastaa `PING`-komentoon vastaavalla `PONG`-komennolla.
* `strcmp_void.c, .h`: käärii `strcmp`-kirjastofunktion jotta se voidaan antaa parametriksi `tsearch`ille ja `tfind`ille,
jotka vaativat muotoa `int (*compar)(const void *, const void *)` olevan vertailufunktion.

`client`-moduulin tiedostot ja niiden tehtävät:

* `clientRead.c, .h`: Callback-funktio joka tulostaa palvelimelta vastaanotetun viestin ja sen ollessa `PING` kutsuu
`replyPing`iä.
* `command_cb.c, .h`: Callback-funktio joka ottaa `stdin`iin kirjoitetut rivin ja suorittaa asianmukaisen toiminnon
  * Jos luettu rivi ei ala `/`, rivi tulkitaan viestiksi ja se lähetetään `PRIVMSG`:lla aktiiviselle kanavalle.
  * Jos luettu rivi alkaa `/`, rivi tulkitaan komennoksi ja lähetetään palvelimelle, paitsi jos komento on
  `MSG_CMD_CHANNEL` tai `MSG_INVALID`. Lisäksi päivitetään tällä hetkellä aktiivinen kanava jos komento on
  `MSG_CMD_CHANNEL` tai `MSG_JOIN`.
* `nick_on_connect_cb.c, .h`: Callback-funktio joka lähettää NICK- ja USER-viestit kun yhteys muodostetaan, muutoin
kutsutaan `close_on_error_eof_cb`tä.
* `main.c`, asiakasohjelman käynnistysfunktio: tulkitsee komentoriviparametrit ja avaa yhteyden.

`server`-moduulin tiedostot ja niiden tehtävät:

* `accept_cb.c, .h`: Callback-funktiot uusien yhteyksien käsittelemistä varten.
* `close_on_error_eof_remove_conn.c, .h`: Käärii ympärilleen `close_on_error_eof_cb`:n poistaen ensiksi yhteyteen
liittyvän `connection`-tietorakenteen ennen `close_on_error_eof_cb`:n kutsumista.
* `broadcastMsg.c, .h`: Lähettää viestin kaikille yhdistetyille asiakkaille.
* `transmitPrivmsg.c, .h`: Lähettää `PRIVMSG`:n kaikille yhdistetyille asiakkaille joita viesti koskee (yksityisviestit
sekä kanavat joille käyttäjä on `JOIN`annut)
* `connection.c, .h`: Tietorakenne jonka avulla pidetään kirjaa yhteyksistä, sekä funktiot niiden hallintaan.
* `doJoin.c, .h`: Funktiot jotka käsittelevät kanavalle liittymisen (`JOIN`) sekä sieltä poistumisen (`PART`).
* `sendWelcome.c, .h`: Lähettää uusille asiakkaille yhteyden avaavan tervetuloviestin.
* `serverRead.c, .h`: `clientRead`in vastine, suorittaa tarvittavat toiminnot vastaanottaessaan asiakkaan viestejä.

Käännös- ja käyttöohjeet
------------------------
Sekä asiakas- että palvelinohjelma käännetään komennolla `make`. Palvelinohjelma käynnistetään komennolla `./homewircd`.
Palvelinohjelma kuuntelee aina yhteyksiä varten kaikista IP-osoitteista oletusportista.

Asiakasohjelma käynnistetään komennolla `./homewirc -s palvelin -n nimimerkki [-p portti]`. Palvelin ja nimimerkki on
pakko antaa komentoriviparametreina, -p on valinnainen ja sen puuttuessa käytetään oletusporttia. Kun yhteys on muodostettu,
voit liittyä kanaville kirjoittamalla `/JOIN #kanava`, sieltä voi poistua `/PART #kanava` ja nimimerkkiä voi vaihtaa
`/NICK nimimerkki`. Viestien lähettäminen tapahtuu kirjoittamalla viesti sellaisenaan omalle rivilleen. Viesti menee
oletuksena sille kanavalle, jolle olet viimeksi `JOIN`annut, mutta sitä voi vaihtaa komentamalla `/CHANNEL #kanava`.
Voit myös lähettää viestejä suoraan toiselle käyttäjälle komentamalla `/CHANNEL nimimerkki`. Huomaa että kaikki komennot
tulee kirjoittaa isolla.

Protokollan kuvaus
------------------
Ohlema toteuttaa [RFC 2812:ssa](https://tools.ietf.org/html/rfc2812) määritellyn IRC-protokollan pääpiirteissään komentojen

* NICK
* USER
* PING
* PONG
* PRIVMSG
* JOIN
* PART
* QUIT
* RPL_WELCOME

osalta. Vastaanottaessaan muunlaisia viestejä palvelin sivuttaa ne kokonaisuudessaan ja asiakas tulostaa ne näytölle.
Palvelimista ei pysty muodostamaan suurempia verkkoja, vaan kaikkien asiakkaiden on yhdistettävä samalle palvelimelle
pystyäkseen keskustelemaan.

Testaus
-------
Projektissa ei ole käytetty yksikkötestejä tms automaattisia testejä, vaan kaikki testaus on suoritettu käsin. Protokollan
toteutuksen oikeellisuutta on testattu käyttämällä palvelinta laajasti käytetyn irssi-IRC-asiakkaan kanssa ja vastaavasti
asiakasta on testattu keskustelemalla IRCNet-, QuakeNet- ja freenode-verkoissa. Asiakasohjelman vakautta testattiin idlaamalla
freenodessa kohtalaista liikennettä saavalla kanavalla noin tunnin ajan. Lisäksi olen testannut erilaisia
poikkeustilanteita esimerkiksi koittamalla lähettää epätavallisia merkkejä kuten kiinalaisia kirjaimia ja tarkistamalla
tulosten olevan toivotunlaisia. Mitään erittäin syvällistä testausta ei kuitenkaan ole suoritettu.
Palvelimessa on pari tunnettua bugia jotka johtuvat siitä, että käyttäjän syötettä ei validoida. Asiakas pystyy kirjoittamaan
`/JOIN nimimerkki`, ja koska kanavan nimeä ei validoida ja yksityisviestit rinnastetaan koodissa kanavaan jolla on sama nimi
kuin käyttäjällä, pystyy kuka tahansa lukemaan toiselle asiakkaalle lukevia yksityisviestejä. Toinen bugi on että annettaessa
komennolle vähemmän parametreja kuin vaadittu syntyy segfault, jälleen kerran johtuen validoinnin puutteesta.


Ohjelman toiminnan analysointi
------------------------------
Valitsin IRC-protokollan koska se on suhteellisen yksinkertainen ja siten helppo toteuttaa, ja laajan levinneisyytensä takia
sitä on myös helppo testata muiden asiakas- ja palvelinohjelmien kanssa ja näin varmistaa yhteensopivuus. Toinen vaihtoehto
olisi ollut kehittää oma protokolla, mutta en valinnut sitä koska vaikka olisin voinut kehittää vielä simplistisemmän
protokollan, olisin joutunut käyttämään aikaa myös protokollan suunnitteluun enkä olisi voinut testata ohjelmaani ulkopuolisten
ohjelmien kanssa. Muita vaihtoehtoja protokollaksi olisi ollut myös esimerkiksi XMPP, mutta muut protokollat ovat sekä
monimutkaisempia että vähemmän levinneitä IRC:iin verrattuna.
Ohjelma toimii testatusti yhtä hyvin sekä IPv4:n että IPv6:n kanssa.
Kun asiakas lopettaa äkisti lähettämisen, niin palvelin sulkee yhteyden normaalisti saatuaan error- tai eof-viestin. Itse asiassa
tässä suhteessa palvelin menettelee lähes identtisesti riippumatta siitä tuliko virhe, EOF vai `QUIT`-komento.
Palvelin pystyy palvelemaan suurtakin asiakasmäärää hyvin: suorituskyky pysyi testissä hyvänä huolimatta noin 1000 samaan aikaan
yhdistäneestä asiakkaasta.
