#!/usr/bin/guile -s
!#

(add-to-load-path (dirname (current-filename)))

(use-modules (ice-9 rdelim)
             (srfi srfi-1)
             (rnrs hashtables)
             (parse))

(define input-file
  (apply (case-lambda ((_) "input")
                      ((_ a . __) a))
         (command-line)))

(define string
  (with-input-from-file input-file
    (lambda () (read-delimited ""))))

(define records (parse-file string))

(define (validate-passport passport)
  (and (fold (lambda (key done)
           (and done (hashtable-ref passport key #f)))
             #t '(byr iyr eyr hgt hcl ecl pid #|cid|#))
       (<= 1920 (string->number (hashtable-ref passport 'byr #f)) 2002)
       (<= 2010 (string->number (hashtable-ref passport 'iyr #f)) 2020)
       (<= 2020 (string->number (hashtable-ref passport 'eyr #f)) 2030)
       (and=> (parse-height (hashtable-ref passport 'hgt #f))
              (lambda (height)
                (case (car height)
                  ((cm) (<= 150 (cdr height) 193))
                  ((in) (<= 59  (cdr height) 76)))))
       (parse-rgb (hashtable-ref passport 'hcl #f))
       (member (hashtable-ref passport 'ecl #f)
               '("amb" "blu" "brn" "gry" "grn" "hzl" "oth"))
       (parse-passport-id (hashtable-ref passport 'pid #f))
       ))

(define (display-passport passport)
  (display
   (string-join
    (map (lambda (key)
           (format #f "~a:~a" key (hashtable-ref passport key #f)))
         (list-head (list-tail (apply circular-list '(byr iyr eyr hcl ecl pid hgt))
                               7) 7))
    " " 'infix)))

(for-each (lambda (passport)
            #;
            (display (if (validate-passport passport)
                         "v \x1b[0;32m"
                         "i \x1b[0;31m"))
            (when (validate-passport passport)
              (display-passport passport)
              (newline))
            #;
            (display "\x1b[m\n")
            )
          records)


(display "number valid = ")
(display (count validate-passport records))
(newline)
