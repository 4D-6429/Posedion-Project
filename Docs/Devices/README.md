# Cihazlar — Bileşen Dökümantasyonu

Her bileşen için ayrı sayfa. Yeni bileşen alındığında bu klasöre ekle, aşağıdaki
listeye işle.

---

## Aktif Bileşenler

### Kontrol & Sensör
- [ESP32_S3_N16R8.md](ESP32_S3_N16R8.md) — Ana MCU
- [FlySky_FS-i6X.md](FlySky_FS-i6X.md) — Kumanda + FS-iA6B alıcı
- [u-blox_NEO-7M.md](u-blox_NEO-7M.md) — GPS modülü

### Güç & Tahrik
- [Masterflight_6S_10000mAh.md](Masterflight_6S_10000mAh.md) — LiPo batarya ×2
- [DEGZ_Guc_Dagitim_Karti.md](DEGZ_Guc_Dagitim_Karti.md) — PDB Hi-Base
- [DEGZ_5V_12V_Regulator.md](DEGZ_5V_12V_Regulator.md) — Ana regülatör
- [Pololu_3V3_Regulator.md](Pololu_3V3_Regulator.md) — MCU regülatör
- [ESC_50A_Bidirectional.md](ESC_50A_Bidirectional.md) — Motor sürücü ×2
- [DEGZ_Ultras_Itici.md](DEGZ_Ultras_Itici.md) — Su altı thruster ×2

### Pasif
- `18AWG_Silikon_Kablo.md` — Sinyal hattı (döküman hazırlanacak; ana güç hattı için 10 AWG kullanılacak)

---

## Eksik / Eklenecek Sayfalar

| Bileşen | Durum |
|---------|-------|
| IMU | Model netleşmedi — seçim sonrası sayfa açılacak |
| OV2640 kamera | Sonra alınacak — sayfa o zaman |
| 4G router | Sonra alınacak — sayfa o zaman |
| ANL Fuse 80A | Alınacak listesinde — sayfa açılabilir |
| XT90 paralel harness | Alınacak listesinde |

---

## Yeni Sayfa Şablonu

```markdown
# [Bileşen Adı] — [Kısa Açıklama]

**Satıcı:**
**Link:**
**Birim Fiyat:**
**Proje Adedi:**

---

## Teknik Özellikler
[tablo]

---

## Proje Notları
[neden seçildi, nereye bağlanıyor, nasıl kullanılıyor]

---

## Bağlantı / Pin Şeması
[varsa pin tablosu]

---

## Uyarılar
[önemli notlar, sınırlar]
```

---

## İlgili

- Genel envanter ve fiyat tablosu: `PROJE_DURUMU.md` (hazırlanacak)
- Pin atamaları: `pin_map.md` (hazırlanacak)
- Bağlantı şeması: `wiring.md` (hazırlanacak)
