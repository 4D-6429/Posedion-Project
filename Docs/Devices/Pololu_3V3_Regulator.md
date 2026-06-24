# Pololu S9V11F3S5C3 — 3.3V Buck-Boost Regülatör

**Üretici:** Pololu  
**Satıcı:** hepsiburada.com (Robotsepeti)  
**Link:** https://www.pololu.com/product/2873  
**Ürün Kodu:** S9V11F3S5C3  
**Birim Fiyat:** 930 TL  
**Proje Adedi:** 1

---

## Teknik Özellikler

| Parametre | Değer |
|-----------|-------|
| Giriş voltajı | 3V – 16V |
| Çıkış voltajı | 3.3V sabit (±5%/-3%) |
| Alternatif çıkış | 5V (SEL pini ile) |
| Max sürekli akım | 1.5A |
| Verimlilik | %85–95 |
| Low voltage cutoff | 3V (histerez: 3.4V) |
| Boyut | 12.7 × 15.3 × 4.3 mm |
| Tip | Buck-Boost (step-up/step-down) |

---

## Özellik: Low Voltage Cutoff

- 3V altına düşünce çıkışı keser → aşırı deşarj koruması
- 3.4V'e kadar histerez — salınım olmadan temiz kesim
- LiPo batarya ömrünü korur

## Özellik: Buck-Boost

- Giriş > çıkış → step-down (buck)
- Giriş < çıkış → step-up (boost)
- 3V girişte bile 3.3V sabit çıkış → batarya bitişinde bile çalışır

---

## Proje Notları

- ESP32-S3 3.3V hattını besleyecek
- DEGZ 5V/12V regülatörün 5V çıkışından veya BEC'ten beslenebilir
- 1.5A limit: ESP32 peak ~500mA, sensörler ~200mA → yeterli
- Boyutu küçük, PCB'ye veya gövdeye kolayca monte edilir
- SEL pini ile 5V moduna geçilebilir (gerekirse)

### Güç Zinciri (Öneri)

```
Batarya (22.2V)
    → PDB
        → ESC ×2 (güç)
        → DEGZ 5V/12V Reg
            → 5V → BEC / RC alıcı
            → 12V → aksesuarlar
            → 5V → Pololu S9V11F3S5C3
                       → 3.3V → ESP32-S3
```

---

## Uyarılar

- Max 16V giriş — 22.2V direkt bağlama, önce 5V/12V regülatörden geçir
- 1.5A limiti aşma — ESP32 + sensör toplamını hesapla
- Küçük boyut, titreşimde gevşemesin, sabit monte et
