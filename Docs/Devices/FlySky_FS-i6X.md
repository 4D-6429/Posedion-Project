# FlySky FS-i6X — 10 Kanal 2.4 GHz Kumanda + FS-iA6B Alıcı

**Proje Adedi:** 1 kumanda + 1 alıcı (FS-iA6B)
**Kullanım:** Manuel pilot kontrol, failsafe sinyali, arm/disarm anahtarı

---

## Kumanda — FS-i6X

| Parametre | Değer |
|-----------|-------|
| Kanal | 10 (alıcıya göre 6/10) |
| Frekans | 2.4 GHz AFHDS 2A |
| Çıkış protokol | PPM, iBUS, S.Bus (mod seçilir) |
| Besleme | 4× AA pil veya 7.4 V LiPo (mod ile) |
| Telemetri | iBUS-sens (geri besleme, opsiyonel) |

---

## Alıcı — FS-iA6B

| Parametre | Değer |
|-----------|-------|
| Kanal | 6 PWM çıkış + iBUS toplu çıkış |
| iBUS pin | SENS/IBUS soketinden |
| PWM pin | CH1..CH6 (kullanmıyoruz, iBUS tek hat) |
| Besleme | 4.0-6.5 V |
| Akım | <100 mA |
| RSSI | iBUS frame içinde |

---

## Kanal Ataması (Projede)

| Kanal | İndex (0-tabanlı) | Fonksiyon | Kumanda | Tip |
|-------|--------------------|-----------|---------|-----|
| CH1 | 0 | (yedek) Roll | Sağ çubuk yatay | Eksen |
| CH2 | 1 | (yedek) Pitch | Sağ çubuk dikey | Eksen |
| CH3 | 2 | **Gaz (throttle)** | Sol çubuk dikey | Eksen |
| CH4 | 3 | **Yaw (dönüş)** | Sol çubuk yatay | Eksen |
| CH5 | 4 | **Arm switch** (planlanan) | SWA / SWD | 2-poz |
| CH6 | 5 | (yedek) | VRA / SWC | Eksen/anahtar |

**Kod referansı:** `Firmware/src/esp32/src/control.c` → `ibus_get_normalized(2)` = CH3 (gaz),
`ibus_get_normalized(3)` = CH4 (yaw).

---

## iBUS Protokol

- **Baud:** 115200
- **Frame:** 32 bayt, header `0x20 0x40`, checksum `0xFFFF - sum(bytes[0..29])`
- **Frame hızı:** ~142 Hz (7 ms periyot)
- **Kanal değeri:** 1000-2000 µs (1500 = merkez)
- **Sinyal voltajı:** 3.3 V → ESP32 GPIO direkt bağlanır (voltaj bölücü gereksiz)
- **Bağlantı:** Alıcı SENS soketi (3 pin: sinyal/+5V/GND) → ESP32 GPIO16

---

## Failsafe Konfigürasyonu (KRİTİK)

FS-iA6B failsafe modunda **frame göndermeye devam eder** — sadece kanal değerleri
preset değerlere düşer. Bizim kodumuz `ibus_is_failsafe()` ile 200 ms frame yokluğunu
yakalar ama bu sadece RF kopması içindir. Alıcı failsafe değerleri **mutlaka
güvenli pozisyona** ayarlanmalı.

### Failsafe Setup Adımları

1. Kumandada Menu → System → RX Setup → Failsafe
2. **CH3 (Gaz) = 1500 µs** (merkez = stop, bidirectional ESC için)
3. **CH4 (Yaw) = 1500 µs** (merkez)
4. **CH5 (Arm) = 1000 µs** (disarm pozisyonu)
5. Diğer kanallar = merkez
6. Save / Confirm
7. **Test:** kumandayı kapat, alıcı LED davranışı kontrol, ESP32 log:
   - 200 ms içinde `ibus_is_failsafe()` true dönmeli
   - control.c → motor_stop() çağrılmalı

---

## Bağlama (Bind) Prosedürü

1. Alıcının **B/VCC** portuna bind plug tak (küçük beyaz 2 pinli soket, SENS ile karıştırma)
2. Alıcıya güç ver — LED hızlı yanıp söner
3. Kumandada Menu → System → RX Bind
4. Kumandayı aç, bağlama otomatik
5. Bind plug'ı çıkar, alıcıyı yeniden başlat
6. LED sabit → bağlantı OK

---

## Telemetri (Opsiyonel — Sonra)

FS-iA6B iBUS-sens (ikinci hat) destekler. Tekneden kumandaya batarya voltajı,
RSSI vb. gönderebiliriz. ESP32 → iBUS-sens TX (half-duplex aynı hat). Şu an
kullanılmıyor.

---

## Uyarılar

- **Sinyal voltajı 3.3 V** — voltaj bölücü kullanma (ESP32 doğrudan)
- **Bind sonrası failsafe set et** — bind reset failsafe'i siler
- Kumanda pili **>%30** → düşükse zayıf sinyal, paket kaybı
- 2.4 GHz alanda WiFi/BT gürültüsü → menzil kontrol et
- Anten **dikey** → max menzil; tekneye göre yatay/dikey pozisyon test
