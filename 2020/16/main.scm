#!/usr/bin/guile -s
!#

(import (ice-9 regex)
        (ice-9 rdelim)
        (srfi srfi-1))

(define predicates
  (map (lambda (line)
         (cond ((string-match
                 "^([a-z ]*): ([0-9]+)-([0-9]+) or ([0-9]+)-([0-9]+)$"
                 line) => (lambda (m)
                            (cons (string->symbol (string-map
                                                   (lambda (c)
                                                     (if (char=? c #\space)
                                                         #\- c))
                                                   (match:substring m 1)))
                                  (lambda (x)
                                    (or (<=
                                         (string->number (match:substring m 2))
                                         x
                                         (string->number (match:substring m 3)))
                                        (<=
                                         (string->number (match:substring m 4))
                                         x
                                         (string->number (match:substring m 5))))))
                            ))
               (else (throw 'match-error))))
       (let loop ()
         (let ((line (read-line)))
           (if (or (eof-object? line)
                   (string-null? line))
               '()
               (cons line (loop)))))))

(read-line)
(define my-ticket (map string->number (string-split (read-line) #\,)))
(read-line)
(read-line)

(define tickets
  (map (lambda (line) (map string->number (string-split line #\,)))
       (let loop ()
         (let ((line (read-line)))
           (if (or (eof-object? line)
                   (string-null? line))
               '()
               (cons line (loop)))))))

(define numbers '())

(define valid-tickets
 (filter-map (lambda (ticket)
               ;; (display ticket) (newline)
               (and (every (lambda (number)
                             ;; true if number valides
                             ;; fales if completely wrong
                             (fold (lambda (a b) (or a b))
                                   #f (map (lambda (p) (p number)) (map cdr predicates))))
                           ticket)
                    ticket))
             tickets))

(define categories (array->list (transpose-array (list->array 2 valid-tickets) 1 0)))

;; (display categories)
;; (newline)

(define valid-tags
 (map (lambda (category i)
        (cons i (filter-map
                 (lambda (pred)
                   (and (fold (lambda (n valid)
                                (and valid ((cdr pred) n)))
                              #t category)
                        (car pred)))
                 predicates)))
      categories
      (iota (length categories))))


(define-once pq-magic (gensym))
(define* (make-priority-queue key compare #:optional (data '()))
  (list pq-magic key compare data))
(define (priority-queue? pq) (eq? pq-magic (car pq)))
(define (priority-queue->list pq)
  (list-ref pq 3))
(define (pq-insert item pq)
  (let ((< (list-ref pq 2))
        (key (list-ref pq 1)))
    (make-priority-queue
     key <
     (let loop ((pq (list-ref pq 3)))
       (cond ((null? pq) (cons item '()))
             ((< (key item) (key (car pq))) (cons item pq))
             (else (cons (car pq)
                         (loop (cdr pq)))))))))


(define pq
  (fold (lambda (item q) (pq-insert item q))
        (make-priority-queue (compose length cdr) <)
        valid-tags))

(define field-types
  (let ((set '()))
   (map (lambda (vtags)
          (let ((return (lset-difference eq? vtags set)))
            (set! set (lset-union eq? set vtags))
            return))
        (priority-queue->list pq))))

(define indices
  (filter-map (lambda (field-type)
                (display field-type) (newline)
                (if (and (<= (string-length "departure")
                            (string-length (symbol->string (cadr field-type))))
                         (string= (symbol->string (cadr field-type))
                                  "departure" 0 (string-length "departure")))
                    (car field-type)
                    #f
                    ;; (begin (display "\x1b[1m") (display (cadr field-type))
                    ;;        (display "\x1b[m") (newline))
                    ;; (begin
                    ;;   (display (cadr field-type)) (newline))
                    ))
              field-types))

(display indices) (newline)
(display my-ticket)
(newline)
(display
 (apply *
  (map (lambda (index) (list-ref my-ticket index))
       indices)))

(newline)

;; (display (length valid-tags))
;; (newline)
;; (display (length (priority-queue->list pq)))

;; (newline)
