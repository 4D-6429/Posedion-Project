# DEGZ Utras Su Altı İticisi

**Üretici:** DEGZ Robotics  
**Satıcı:** mucif.com  
**Link:** https://www.mucif.com/urunler/degz-ultras-su-alti-iticisi/  
**Birim Fiyat:** 15,967.86 TL (KDV dahil)  
**Proje Adedi:** 2

---

## Teknik Özellikler

| Parametre | Değer |
|-----------|-------|
| İtki gücü | **8 Kgf @ 24V (6S)** |
| Voltaj aralığı | 3S–6S (12V – 24V) |
| Max sürekli akım | **35A** (üzeri uzun süre önerilmez) |
| Max derinlik | 500 m |
| Motor | M5 fırçasız (brushless) |
| Gövde | Poliüretan (enjeksiyon kalıplama) |
| Tutucu | Uçak sınıfı alüminyum |
| Montaj | M5×16 havşabaş vida |
| Kontrol | PWM sinyali |
| Pervane | Poliüretan, düşük gürültü optimize |

---

## Kontrol Uyumu

- Arduino, Raspberry Pi, ESP32 uyumlu
- Standart ESC PWM protokolü
- 3D tasarım dosyaları ve STEP modelleri mevcut

---

## Proje Notları

- 2 adet — her hull'a 1 itici
- 22.2V (6S) sistemle tam voltajda çalışır → 8 Kgf thrust
- 50A ESC ile kullanılıyor — motor max 35A, ESC yeterli marjda
- Diferansiyel thrust ile yönlendirme: sol/sağ farklı hız → dönüş (rudder gereksiz)
- Toplam max thrust: **16 Kgf** (2 × 8 Kgf) — USV ağırlığına göre değerlendir

### Güç Hesabı

| Senaryo | Akım (per itici) | Güç (per itici) | Toplam |
|---------|------------------|-----------------|--------|
| Full throttle | 35A | 777W | 1,554W |
| Cruise %60 | ~21A | ~466W | ~932W |
| Yavaş %30 | ~10A | ~222W | ~444W |

---

## Bakım

- Deniz suyu kullanımı sonrası **tatlı suyla çalıştır** (tuz kalıntısı temizleme)
- Kırılmazlık garantisi: hasar görülen parça **ücretsiz değişim**

---

## Uyarılar

- **35A üzeri uzun süreli çalışmadan kaçın** — motor ısınır
- ESC motor kablosu (3 faz) doğru sırayla bağlanmalı
- Güç kablosunda min **12 AWG** kullan (listede 18 AWG var — yetersiz)
