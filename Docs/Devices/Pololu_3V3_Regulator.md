---
part: Pololu-S9V11F3S5C3
category: regulator
supplier: hepsiburada.com
supplier_url: https://www.pololu.com/product/2873
price_tl: 930
qty: 1
status: alındı
---

# Pololu S9V11F3S5C3 — 3.3 V Buck-Boost Regülatör

> ESP32-S3'ün 3.3 V besleme hattı. DEGZ 5 V regülatöründen beslenir; düşük voltaj kesme özelliği ile batarya koruması sağlar.

| | |
|-|-|
| Üretici | Pololu |
| Ürün Kodu | S9V11F3S5C3 |
| Satıcı | hepsiburada.com (Robotsepeti) |
| Birim Fiyat | 930 TL |
| Proje Adedi | 1 |
| Durum | Alındı |

---

## Teknik Özellikler

| Parametre | Değer |
|-----------|-------|
| Giriş voltajı | 3–16 V |
| Çıkış voltajı | 3.3 V sabit (tolerans: +5% / −3%) |
| Alternatif çıkış | 5 V (SEL pini ile seçilir) |
| Max sürekli akım | 1.5 A |
| Verimlilik | %85–95 |
| Low voltage cutoff | 3 V (histerez: 3.4 V) |
| Boyut | 12.7 × 15.3 × 4.3 mm |
| Tip | Buck-Boost (step-up / step-down) |

---

## Özellik: Buck-Boost

Giriş voltajı çıkış voltajından yüksekse buck (düşürür), düşükse boost (yükseltir) modunda çalışır. Bu sayede 3 V girişte bile 3.3 V sabit çıkış sağlanır — batarya deşarj sonuna yaklaşırken bile ESP32 çalışmaya devam eder.

## Özellik: Low Voltage Cutoff

Giriş 3 V'un altına düşünce çıkışı keser. 3.4 V'a çıkana kadar yeniden açılmaz (histerez). LiPo bataryanın aşırı deşarjını önler.

---

## Güç Zinciri

```
Batarya (22.2 V)
    └── PDB
          └── DEGZ 5V/12V Reg
                    └── 5 V çıkış
                              └── Pololu S9V11F3S5C3
                                        └── 3.3 V → ESP32-S3
```

> **Önemli:** Pololu max 16 V giriş kabul eder. Batarya voltajını (22.2 V) doğrudan bağlama.

---

## Yük Tahmini

| Yük | Akım |
|-----|------|
| ESP32-S3 (aktif, Wi-Fi kapalı) | ~150 mA |
| ESP32-S3 (Wi-Fi aktif, peak) | ~500 mA |
| Sensörler (IMU, GPS) | ~100–200 mA |
| **Toplam tahmini** | **~700 mA** |

1.5 A limite karşı yeterli marj var.

---

## Uyarılar

- Giriş voltajı max **16 V** — DEGZ 5 V hattından besle, bataryadan doğrudan bağlama
- 1.5 A limitini aşma — eklenecek her sensör için yük hesabını güncelle
- Küçük boyutu nedeniyle titreşimde gevşeyebilir — rijit monte et
- SEL pini 5 V modunu etkinleştirir; varsayılan konumda 3.3 V çıkış geçerlidir
