# u-blox NEO-7M GPS Modülü

**Proje Adedi:** 1
**Kullanım:** Pozisyon, hız, heading (sürat üzerinden), zaman senkronizasyonu

---

## Teknik Özellikler

| Parametre | Değer |
|-----------|-------|
| Çip | u-blox NEO-7M |
| GNSS | GPS (L1, C/A) — GLONASS opsiyonel mod |
| Hassasiyet (CEP) | 2.5 m otomatik, 2.0 m SBAS aktifken |
| Update rate | 1-10 Hz (default 1 Hz, 5 Hz kullanılması önerilir) |
| TTFF (cold) | ~26 s |
| TTFF (hot) | ~1 s |
| Besleme | 3.3 V veya 5 V (modüle göre — modülün üzerindeki regülatöre bak) |
| Akım | ~50 mA (acquisition), ~30 mA (tracking) |
| Anten | Aktif seramik patch (modül üzerinde) |
| Arayüz | UART (NMEA veya UBX) |
| Default baud | 9600 (modüle göre değişir) |
| EEPROM | Var — konfig kalıcı |
| Yedek pil | CR1220 (hot-start için) |

---

## Bağlantı

| GPS pin | ESP32-S3 | Not |
|---------|----------|-----|
| VCC | 5V veya 3.3V | Modüldeki regülatöre göre |
| GND | GND | Ortak |
| TX | UART RX (TBD) | NMEA out |
| RX | UART TX (TBD) | u-center konfig için (opsiyonel) |

**Atanmamış pinler için:** `doc/pin_map.md`

---

## NMEA Cümleleri (Default Output)

```
$GPGGA  — pozisyon, irtifa, sabit kalite, uydu sayısı
$GPRMC  — pozisyon, sürat, heading, tarih, zaman (en faydalı)
$GPGSA  — DOP değerleri, sabit modu
$GPGSV  — uydu detayları
$GPVTG  — sürat ve heading (yer üzerinde)
$GPGLL  — pozisyon ve zaman
```

**Proje için minimal:** GGA + RMC yeter. Diğerlerini kapatarak baud kazanılır.

---

## Konfigürasyon (u-center ile)

İlk kullanımda **u-center** (Windows tool) ile bir kez yapılandır:

1. USB-UART adaptör ile bilgisayara bağla
2. u-center aç, port + baud (9600) seç
3. View → Messages View → NMEA cümlelerini gör
4. View → Configuration View:
   - **NMEA:** sadece GGA + RMC aktif (diğerleri 0)
   - **Rate:** 200 ms (5 Hz)
   - **PRT:** baud 115200 yap (daha hızlı veri)
   - **SBAS:** EGNOS aktif (Avrupa, +0.5 m hassasiyet)
   - **CFG:** Save → Save current configuration (EEPROM'a yaz)

Konfigürasyon EEPROM'a yazılınca güç kesilse de korunur.

---

## Anten Yerleşimi

- **Gökyüzü görüşü açık** — muhafaza dışında veya RF şeffaf panel altında
- Metal yüzeylerden uzak (en az 5 cm)
- ESP32, ESC, motor gibi gürültü kaynaklarından **uzak** (>20 cm)
- Yatay yönlü monte — patch antenin üst yüzü gökyüzüne baksın

---

## Code Snippet (Planlanan)

```c
/* Plan — henüz implemente değil */
#include "driver/uart.h"

uart_config_t cfg = {
    .baud_rate = 115200,  /* veya 9600 (default) */
    .data_bits = UART_DATA_8_BITS,
    .parity    = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
};
uart_param_config(UART_NUM_2, &cfg);
uart_set_pin(UART_NUM_2, GPS_TX_GPIO, GPS_RX_GPIO, -1, -1);
uart_driver_install(UART_NUM_2, 512, 0, 0, NULL, 0);

/* NMEA cümle parse → minmea kütüphanesi önerilir */
```

---

## Beklenen Performans (Saha)

- **Açık alan / deniz:** 6-12 uydu, HDOP 0.8-1.5, ±2 m
- **Şehir / engelli:** 4-8 uydu, HDOP 2-5, ±5-10 m
- **HDOP >3 → güvenilmez**, waypoint takibi yapma

---

## Uyarılar

- **İlk fix soğuk başlatma:** açık alanda **2-5 dk** bekle (TTFF)
- **CR1220 backup pil** boşaldıysa her seferde cold start (yavaş) — pili değiştir
- **5V besleme zorunlu** olabilir (modüldeki regülatör 3.3V'a düşürür)
- **NEO-7 dahili pusula içermez** — heading sadece hareket sırasında (>0.5 m/s)
- **NEO-7 yerine NEO-8M/NEO-9M** alternatifi daha yeni, GLONASS+Galileo destekli
- Anten kablosu uzunsa düşük gürültülü (LMR-100 gibi) koaks kullan

---

## Açık Sorular

- [ ] Modül üzerindeki regülatör 3.3V mı 5V mu girdi bekliyor? → datasheet/satıcı kontrol
- [ ] Dahili pusula var mı (mıknatıs sensör)? → NEO-7M default yok
