# Bidirectional ESC 50A+ — Su Altı Motoru Uyumlu

> **NOT:** Orijinal listeki f1depo.com 50A ESC stokta yok. Alternatif seçenekler aşağıda.

**Proje Adedi:** 2 (3. ESC iptal edildi)

---

## Gereksinimler

| Parametre | Minimum |
|-----------|---------|
| Voltaj | **6S uyumlu (25.2V+)** |
| Sürekli akım | **35A+** |
| Yön | **Bidirectional** |
| Kontrol | PWM (1000–2000 µs) |

---

## Alternatif Seçenekler

### Seçenek 1 — Bidirectional ESC 60A (Önerilen)
- **Satıcı:** robotzade.com
- **Link:** https://www.robotzade.com/urun/bidirectional-esc-60a-su-alti-motoru-ile-uyumlu
- **Fiyat:** 1,386 TL
- **Voltaj:** 2–6S ✓
- **Akım:** 60A ✓
- **Yön:** Bidirectional ✓

### Seçenek 2 — Bidirectional ESC 50A (Motorobit)
- **Satıcı:** motorobit.com
- **Link:** https://www.motorobit.com/bidirectional-esc-50a-compatible-with-underwater-motor
- **Voltaj:** 2–6S ✓ (doğrulama önerilir)
- **Akım:** 50A ✓

### Seçenek 3 — VESC 6.7 70A (Flipsky) — Gelişmiş
- **Voltaj:** 14–60V ✓
- **Akım:** 70A sürekli, 200A anlık
- **Protokol:** PWM + UART + CAN
- **Telemetri:** RPM, akım, sıcaklık → ESP32'ye
- **Fiyat:** ~3,000–5,000 TL
- **Not:** UART üzerinden ESP32 entegrasyonu mükemmel

---

## PWM Kontrol

| Değer | Komut |
|-------|-------|
| 1000 µs | Tam geri |
| 1500 µs | Dur (nötr) |
| 2000 µs | Tam ileri |

---

## Bağlantı

```
Batarya → PDB → ESC → Utras İtici
                 ↑
           ESP32 GPIO PWM
```

---

## Uyarılar

- **ESC kalibrasyonu şart:** İlk açılışta 1500µs gönder, 3 sn bekle
- Ana hat için **min 12 AWG** kablo kullan
- Motor 3 faz kablosu (A-B-C) yanlış sırada → motor titrer, iki kabloyu değiştir
- Muhafaza dışına al — uzun çalışmada ısınır
