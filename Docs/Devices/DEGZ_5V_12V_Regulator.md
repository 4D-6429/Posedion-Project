# DEGZ 5V / 12V Gerilim Regülatörü

**Üretici:** DEGZ Robotics  
**Satıcı:** mucif.com  
**Link:** https://www.mucif.com/urunler/5v-12v-gerilim-regulatoru/  
**Birim Fiyat:** 1,403.53 TL  
**Proje Adedi:** 1

---

## Teknik Özellikler

| Parametre | Değer |
|-----------|-------|
| Giriş voltajı | 8–36V (3S–8S LiPo) |
| Çıkış 1 | 5V / 5A |
| Çıkış 2 | 12V / 4A |
| Konfigürasyon | 2× 5V, 2× 12V, veya 1×5V + 1×12V |
| Ters bağlantı koruması | **YOK** |
| Tip | DC-DC Buck (step-down) |

---

## Proje Notları

- 22.2V (6S) girişten ESP32 (5V) ve sensörler (12V) besleyecek
- 5V çıkış → ESP32, RC alıcı, servo
- 12V çıkış → kamera, aydınlatma, vb. (varsa)
- Güç Dağıtım Kartı'ndan sonra konumlandırılmalı
- İki çıkış bağımsız, yük dağıtımı esnek

---

## Uyarılar

- Ters bağlantı koruması YOK — bağlantı öncesi polarite kontrol et
- 5A / 4A çıkış limitlerini aşma
- Isınma takibi yap, gerekirse soğutucu ekle
