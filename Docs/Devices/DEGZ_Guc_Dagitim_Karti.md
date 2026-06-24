# DEGZ Güç Dağıtım Kartı — Hi-Base (Hall Effect Sensör)

**Üretici:** DEGZ Robotics  
**Satıcı:** mucif.com  
**Link:** https://www.mucif.com/urunler/degz-guc-dagitim-karti-hi-base-hall-effect-sensor/  
**Birim Fiyat:** 4,693.65 TL  
**Proje Adedi:** 1

---

## Teknik Özellikler

| Parametre | Değer |
|-----------|-------|
| Max akım (ana hat) | 200A |
| Boyut | 45 × 45 mm |
| Voltaj sensörü | JST çıkış, gerçek zamanlı |
| Akım sensörü | Hall Effect, JST çıkış |
| Sıcaklık sensörü | Termistör, JST çıkış |
| On/Off kontrolü | JST çıkış (uzaktan) |
| Bağlantı sayısı | Sınırsız (200A limit dahilinde) |

---

## JST Çıkışları

| Port | Ölçüm |
|------|--------|
| Voltaj | Batarya gerilimi (anlık) |
| Hall Effect | Akım yönü + şiddeti |
| Termistör | Sıcaklık |
| On/Off | Kart açma/kapama |

---

## Proje Notları

- Batarya → PDB → ESC zincirinde merkez nokta
- Hall Effect sensör verisi ESP32'ye gönderilip batarya takibi yapılabilir
- 200A limit: 2× ESC 50A = 100A max, geniş marj var
- On/Off JST → kill switch veya ESP32 GPIO ile kontrol
- 45×45mm kompakt, gövdeye kolay yerleşir

---

## Uyarılar

- Su geçirmez muhafazaya alınmalı (IP67+)
- JST kablo bağlantıları düzgün haritalanmalı
- Ana hat kablosu 10AWG veya kalın olmalı (listede eksik)
