(use-modules (cairo)
             (srfi srfi-9)
             ((rnrs base) :select (mod))
             (ice-9 format)
             )

;; H := [0, 360)
;; S := [0, 1] ; hur mättad (lågt == blekt)
;; V := [0, 1] ; "ljus" (lågt == mörkt)
(define-record-type <hsv>
  (hsv h s v)
  hsv?
  (h h) (s s) (v v))

(define-record-type <rgb>
  (rgb r g b)
  rgb?
  (r r) (g g) (b b))

(define (hsv->rgb hsv)
  (define c (* (v hsv)
               (s hsv)))
  (define x (* c (- 1 (abs (- (mod (/ (h hsv) 60) 2) 1)))))
  (define m (- (v hsv) c))

  (call-with-values
      (lambda ()
        (cond ((< (h hsv) 60)  (values c x 0))
              ((< (h hsv) 120) (values x c 0))
              ((< (h hsv) 180) (values 0 c x))
              ((< (h hsv) 240) (values 0 x c))
              ((< (h hsv) 300) (values x 0 c))
              ((< (h hsv) 360) (values c 0 x))))
    (lambda (r* g* b*)
      (rgb (* #xFF (+ r* m))
           (* #xFF (+ g* m))
           (* #xFF (+ b* m))))))

(define (rgb->hsv rgb)
  (define r* (/ (r rgb) #xFF))
  (define g* (/ (g rgb) #xFF))
  (define b* (/ (b rgb) #xFF))
  (define c-max (max r* g* b*))
  (define delta (- c-max (min r* g* b*)))

  (define h (* 60
               (cond ((= 0 delta) 0)
                     ((= r* c-max) (mod (/ (- g* b*) delta) 6))
                     ((= g* c-max) (+ 2 (/ (- b* r*) delta)))
                     ((= b* c-max) (+ 4 (/ (- r* g*) delta))))))
  (define s (if (= 0 c-max) 0 (/ delta c-max)))
  (define v c-max)

  (hsv h s v))

(define (rgb->fractionals rgb)
  (map (lambda (x) (exact->inexact (/ x #xFF)))
       (list (r rgb) (g rgb) (b rgb))))



(define fl (compose exact->inexact floor ))






(define height 1000)
(define phi 1.61803)
(define width (inexact->exact (floor (* height phi))))




(define surf (cairo-image-surface-create 'argb32 width height))
(define cr (cairo-create surf)) ;; context

;; make source

(cairo-select-font-face cr "Bitstream Vera Sans" 'normal 'normal)
;; (cairo-scale cr height height)

(define background (rgb->hsv (rgb #xB2 #x9F #x90)))

(apply cairo-set-source-rgb cr
       (rgb->fractionals (hsv->rgb background)))

(cairo-rectangle cr 0 0 (* width 2) (* height 2))
;; (cairo-paint cr)
(cairo-fill cr)

;; (cairo-new-path cr)
(cairo-rectangle cr
                 (fl (/ height 20)) (fl (/ height 20))
                 (fl (* 7 (/ height 10)))
                 (fl (* 9 (/ height 10)))
                 )

(apply cairo-set-source-rgb cr
       (rgb->fractionals
        (hsv->rgb
         (hsv (h background)
              (max (- (s background) 0.0) 0)
              (max (- (v background) 0.2) 0)
              ))))

(cairo-fill cr)

;; (cairo-mask cr pat)

;; (cairo-paint cr)


;; make mask

(define fields
 `((byr 1998)
   (iyr 2015)
   (eyr 2025)
   (hgt (cm . 170))
   (hcl ,(rgb #x00 #xCC #x00))
   (ecl "brn")
   (cid "SE")))

(define
  pid 123456789)

(define fieldnames
  `((byr "Födelseår")
    (iyr "Utfärdat")
    (eyr "Går ut")
    (hgt "Höjd")
    (hcl "Hårfärg")
    (ecl "Ögonfärg")
    ;; (pid "Passnummer")
    (cid "Landskod")))




(define pat (cairo-pattern-create-linear 0 0 1000 1000))
(cairo-pattern-add-color-stop-rgba pat 1000 1 0 0 1)
(cairo-pattern-add-color-stop-rgba pat 0 0 0 1 1)
(cairo-set-source cr pat)

;; (cairo-set-font-size cr (/ height 2))
(cairo-set-font-size cr 60)

(for-each (lambda (field i)
            (define key (car field))
            (define value (cadr field))
            (define str (car (assoc-ref fieldnames key)))

            (define line (* i (/ (* 9/10 height) (length fields))))

            (cairo-move-to cr (/ width 2) line)
            (cairo-show-text cr str)

            (cairo-move-to cr (+ (/ width 2) (/ width 4))
                           line)
            (cairo-show-text
             cr
             (case key
               ((byr iyr eyr) (number->string value))
               ((hgt) (format #f "~a~a" (cdr value) (car value)))
               ;; ((pid) (format #f "~9,'0d" value))
               ((hcl)
                (format #f "~2,'0X, ~2,'0X, ~2,'0X"
                        (r value) (g value) (b value)))
               ((ecl) (case (string->symbol value)
                        ((amb) "Bärnsten")
                        ((blu) "Blå")
                        ((brn) "Brun")
                        ((gry) "Grå")
                        ((grn) "Grön")
                        ((hzl) "Hasell")
                        ((oth) "Annan")))
               ((cid) (format #f "~a" value)))))
          fields
          (iota (length fields)))

(cairo-set-font-size cr 40)
(cairo-set-source-rgb cr 0 0 0)
(cairo-move-to cr (/ width 2)
               (- height (* height 1/20)))
(define pidstr (let ((str (format #f "~9,'0d" pid)))
                 (string-join
                  (list
                   (substring str 0 3)
                   (substring str 3 6)
                   (substring str 6 9))
                  " " 'infix)))
(cairo-show-text cr pidstr)

(cairo-set-font-size cr 100)
;; TODO replace with wax noice
(cairo-set-source-rgb cr 1 0 0)
(cairo-move-to cr 100 100)
(cairo-rotate cr (* 3.141592653589793 0.25))
(cairo-show-text cr "DENIED")

;; write out

(cairo-surface-write-to-png surf "guile-cairo.png")


;; byr - födelseår
;; iyr - utfärdat
;; eyr - går ut
;; hgt - längd
;; hcl - hårfärg
;; ecl - ögonfärg
;; pid - pass #
;; cid - landskod
