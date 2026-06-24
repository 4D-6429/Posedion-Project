# Masterflight 10000 mAh 65C 6S 22.2 V LiPo

**Proje Adedi:** 2 (paralel bağlanacak — toplam 20000 mAh @ 6S)
**Konnektör:** XT90

---

## Teknik Özellikler

| Parametre | Değer |
|-----------|-------|
| Hücre sayısı | 6S (6 hücre seri) |
| Nominal voltaj | 22.2 V |
| Tam dolu | 25.2 V (4.20 V/hücre) |
| Boş (cutoff) | 19.8 V (3.30 V/hücre) — daha aşağı **inme** |
| Storage | 22.8-23.1 V (3.80-3.85 V/hücre) |
| Kapasite | 10000 mAh = 10 Ah |
| C-rating | 65C (sürekli), 130C (anlık) |
| Max sürekli akım | 650 A (teorik — 35A motor için fazlasıyla yeterli) |
| Ağırlık | ~1400 g/adet (datasheet doğrula) |
| Konnektör | XT90 (yüksek akım) |
| Balance konnektör | JST-XH 7-pin (6S+GND) |

---

## Enerji ve Süre

- **Enerji (tek batarya):** 22.2 V × 10 Ah = **222 Wh**
- **Enerji (paralel ikili):** **444 Wh**

| Senaryo | Toplam güç | Süre (444 Wh) |
|---------|-----------|---------------|
| Full throttle (2× 777W) | 1554 W | ~17 dk |
| Cruise %60 | 932 W | ~28 dk |
| Yavaş %30 | 444 W | ~60 dk |
| Idle (sadece elektronik) | ~5 W | >70 saat |

> Süreler **%100 → %20 deşarj** baz alınarak hesaplandı (3.50V/hücre cutoff).
> %0'a inmek hücreyi öldürür, asla yapma.

---

## Paralel Bağlama

**XT90 paralel harness** (2→1) kullanılır. **ÖNEMLİ:**

1. Bağlamadan önce her iki bataryanın voltajı **eşit** olmalı (max **0.1 V fark**)
2. Voltaj farkı varsa balance şarj ile eşitle — direkt paralel = arc + akım şoku
3. Paralel bağlandıktan sonra **tek batarya gibi** davranır
4. **Asla seri bağlama** — proje 6S, paralel kapasite artırır, voltaj artırmaz
5. Harness'i tekne tarafına bağlamadan önce iki bataryayı birbirine bağla

---

## Şarj Prosedürü

| Parametre | Değer |
|-----------|-------|
| Şarj akımı (önerilen) | 5 A (0.5C, güvenli) |
| Şarj akımı (max) | 10 A (1C, hızlı) |
| Balance şarj | **ZORUNLU** her seferinde |
| Şarj sıcaklığı | 0-40 °C |
| Tam dolu voltaj | 4.20 V/hücre (±0.02 V) |
| Hücreler arası fark | <50 mV (tam dolu sonrası) |

**Adımlar:**
1. Şarj cihazı **LiPo balance** moduna al
2. Cell count: **6S** (otomatik tanıma ile doğrula)
3. Şarj akımı = 5 A (acil değilse)
4. Balance konnektörü **mutlaka** bağla
5. Şarj başlat — **GÖZLEM ALTINDA**, asla yalnız bırakma
6. Tamamlanınca 10 dk bekle, hücre voltajlarını oku

---

## Depo (Storage)

Pil 24 saatten uzun süre kullanılmayacaksa **mutlaka storage voltajına** ayarla.

- **Storage voltaj:** 3.80-3.85 V/hücre = ~22.8-23.1 V toplam
- Şarj cihazında "Storage" / "Discharge to storage" modu
- Tam dolu bırakılan LiPo → şişme + kapasite kaybı → ölüm
- Boş bırakılan LiPo → derin deşarj → ölüm

---

## Test Öncesi Kontrol

- [ ] Hücre voltajı **>4.10 V/hücre** (tam doluya yakın)
- [ ] Hücreler arası fark **<50 mV**
- [ ] Sıcaklık ortam sıcaklığında
- [ ] Şişme/balon yok (gözle ve elle yokla)
- [ ] Kablo/konnektör hasar yok
- [ ] Test bitiminde voltaj **>3.50 V/hücre** kalmalı, daha düşükse durdur

---

## Uyarılar

### Yangın Riski
- LiPo yangını **su ile söndürülmez** — kum, toprak veya class D söndürücü
- Şarj sırasında **fireproof bag** zorunlu
- Yanıcı yüzey üzerinde **şarj etme**

### Şişme Tespit
Şişmiş batarya = ÖLÜ. Şarj etme, kullanma.
1. Tuzlu suya batır (3-4 yemek kaşığı tuz / 5 L su)
2. **1 hafta** beklet — tam deşarj olur, voltaj 0 V
3. Belediye e-atık veya pil geri dönüşüm noktası

### Taşıma
- Konnektörler **kapalı/izole** (kısa devre)
- Sert plastik kutu içinde
- Uçakla taşıma → havayolu kuralları, LiPo özel prosedür

### Çevresel
- Bataryalar **toprak/su'ya atılmaz**
- Pil geri dönüşüm noktası: AVM'ler, elektronik mağazaları
