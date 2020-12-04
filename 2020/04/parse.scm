(define-module (parse)
  :export (parse-file parse-height
                      parse-rgb
                      parse-passport-id))

(use-modules (ice-9 peg)
             (rnrs hashtables))


(define-peg-pattern ws none (or " " "\n"))
(define-peg-pattern record all (and field (+ (and (ignore ws) field))))

(define-peg-pattern key all (+ (range #\a #\z)))

(define-peg-pattern value all (+ (and (not-followed-by ws) peg-any)))

(define-peg-pattern field all (and key (ignore ":") value))

(define-peg-pattern file body
  (and record
       (+ (and (ignore "\n\n") record))))


(define (build-struct record)
  (let ((ht (make-eq-hashtable)))
    (for-each (lambda (field)
                (let ((key (car (assoc-ref (cdr field) 'key)))
                      (value (car (assoc-ref (cdr field) 'value))))
                  (hashtable-set! ht (string->symbol key) value)))
              (cdr record))
    ht))

(define (parse-file string)
  (map build-struct
   (map (lambda (record) (keyword-flatten '(field) record))
        (keyword-flatten
         '(record)
         (peg:tree (match-pattern file string))))))


(define-peg-pattern unit all
  (or "cm" "in"))

(define-peg-pattern digit body
  (range #\0 #\9))

(define-peg-pattern number all
  (+ digit))

(define-peg-pattern height all (and number unit))

(define (parse-height string)
  (and=> (peg:tree (match-pattern height string))
         (lambda (h)
           (let ((unit (assoc-ref (cdr h) 'unit))
                 (number (assoc-ref (cdr h) 'number)))
             (and unit number
                  (cons (string->symbol (car unit))
                        (string->number (car number))))))))


(define-peg-pattern hex-digit body
  (or (range #\0 #\9)
      (range #\a #\f)
      (range #\A #\F)))

(define-peg-pattern rgb-number all
  (and (ignore "#")
       ;; TODO nicer way to match 6 times.
       ;; since compiled peg patterns are simply procedurs
       ;; some form of meta procedure would work. Problem
       ;; being that the body of a define-peg-pattern is
       ;; NOT scheme code.
       hex-digit
       hex-digit
       hex-digit
       hex-digit
       hex-digit
       hex-digit
       (not-followed-by hex-digit)
       ))

(define (parse-rgb string)
  (and=> (peg:tree (match-pattern rgb-number string))
         (lambda (tree) (string->number (cadr tree) 16))))


;; exactly 9 digits
(define-peg-pattern passport-id all
  (and digit
       digit
       digit
       digit
       digit
       digit
       digit
       digit
       digit
       (not-followed-by digit)))

(define (parse-passport-id string)
  (and=> (peg:tree (match-pattern passport-id string))
         (lambda (tree)
           (string->number (cadr tree)))))
