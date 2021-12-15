;;; segments-lit: which numbers
;;; 2: 1
;;; 3: 7
;;; 4: 4
;;; 5: 2, 3, 5
;;; 6: 9, 6, 9
;;; 7: 8
;;; 
;;;  aaa
;;; b   c
;;; b   c
;;; b   c
;;;  ddd
;;; e   f
;;; e   f
;;; e   f
;;;  ggg


(use-modules (ice-9 rdelim)
             (srfi srfi-1)
             (ice-9 hash-table))

;; acedgfb cdfbe gcdfa fbcad dab cefabd cdfgeb eafb cagedb ab
;; cdfeb fcadb cdfeb cdbaf


(define (group by lst)
  (define ht (make-hash-table))
  (for-each (lambda (item) (hash-set! ht (by item) (cons item (hash-ref ht (by item) '()))))
            lst)
  (hash-map->list cons ht))

;; (zip lefts (map string-length lefts))
;; => (("acedgfb" 7) ("cdfbe" 5) ("gcdfa" 5) ("fbcad" 5) ("dab" 3) ("cefabd" 6) ("cdfgeb" 6) ("eafb" 4) ("cagedb" 6) ("ab" 2))
;; (group string-length lefts)
;; => ((4 "eafb") (6 "cagedb" "cdfgeb" "cefabd") (7 "acedgfb") (5 "fbcad" "gcdfa" "cdfbe") (3 "dab") (2 "ab"))

(define (parse-lefts lefts)
  (define groups (group length lefts))

  (define a (car (lset-difference char=?
                                  (car (assoc-ref groups 3))
                                  (car (assoc-ref groups 2)))))

  ;; d or g
  (define d-or-g
    (lset-difference
     char=?
     (apply lset-intersection
            char=?
            (assoc-ref groups 5))
     (list a)))
  ;; => (#\f #\c)

  ;; c, d, or e
  (define c-or-d-or-e
    (lset-difference
     char=?
     (car (assoc-ref groups 7))
     (apply lset-intersection
            char=?
            (assoc-ref groups 6))))
  ;; => (#\a #\g #\f)

  (define d (car (lset-intersection char=? d-or-g c-or-d-or-e)))
  (define g (car (lset-difference char=? d-or-g (list d))))

  (define c-or-e (lset-difference char=? c-or-d-or-e (list d)))

  (define c (car (lset-intersection char=? c-or-e (car (assoc-ref groups 4)))))
  (define e (car (lset-difference char=? c-or-e (list c))))

  (define f (car (lset-difference char=?
                                  (car (assoc-ref groups 2))
                                  (list c))))
  (define b (car (lset-difference char=?
                                  (car (assoc-ref groups 7))
                                  (list a c d e f g))))


  (define lookup
    (alist->hashq-table
     `((,a . #\a)
       (,b . #\b)
       (,c . #\c)
       (,d . #\d)
       (,e . #\e)
       (,f . #\f)
       (,g . #\g))))

  lookup)


(define lit-segments-list
  `((0 "abcefg")
    (1 "cf")
    (2 "acdeg")
    (3 "acdfg")
    (4 "bcdf")
    (5 "abdfg")
    (6 "abdefg")
    (7 "acf")
    (8 "abcdefg")
    (9 "abcdfg")))

(define decode-table
  (alist->hash-table
   (map (lambda (pair)
          (cons (sort (string->list (cadr pair)) char<?)
                (car pair)))
        lit-segments-list)))

(define (list->number lst)
  (fold (lambda (p total)
          (define base (car p))
          (define exponent (cadr p))
          (+ total (* base (expt 10 exponent))))
        0
        (zip (reverse lst)
             (iota (length lst)))))

(define total
 (let loop ((total 0))
   (let ((line (read-line)))
     (if (eof-object? line)
         total
         (let ()
           (define pair (remove string-null? (map string-trim-both (string-split line #\|))))
           (define lefts (car pair))
           (define rights (cadr pair))
           (define lookup (parse-lefts (map string->list (string-split lefts #\space))))
           (define num (list->number
                        (map (lambda (s)
                               (hash-ref decode-table
                                         (sort (map (lambda (c) (hashq-ref lookup c)) (string->list s))
                                               char<?)
                                         ))
                             (string-split rights #\space))))
           (format #t "~a: ~a~%" rights num)
           (loop (+ total num)))))))

(format #t "total = ~a~%" total)

;; (define lefts (map string->list '("acedgfb" "cdfbe" "gcdfa" "fbcad" "dab" "cefabd" "cdfgeb" "eafb" "cagedb" "ab")))


