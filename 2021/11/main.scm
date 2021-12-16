#!/usr/bin/guile \
-e main -s
!#

(use-modules ((srfi srfi-41)
              :renamer (lambda (s) (case s
                                ((port->stream) (string->symbol (string-append "srfi:" (symbol->string s))))
                                (else s))))
             (srfi srfi-1)
             (ice-9 rdelim)
             (ice-9 streams)
             (ice-9 control)
             (ice-9 match)
             (sdl2)
             (sdl2 video)
             (sdl2 rect)
             (sdl2 events)
             (sdl2 surface)
             (sdl2 ttf)
             (sdl2 render))

(define (cross-product l1 l2)
  (concatenate (map (lambda (a)
                      (map (lambda (b) (list a b))
                           l2))
                    l1)))

(define-syntax-rule (label self proc)
  (letrec ((self proc)) proc))

(define-syntax ->>
  (syntax-rules ()
    ((->> obj)
     obj)
    ((->> obj (func args ...) rest ...)
     (->> (func args ... obj) rest ...))
    ((->> obj func rest ...)
     (->> (func obj) rest ...))))

(define (neighbours x y)
  (remove (match-lambda ((xx yy) (and (= xx x)
                                      (= yy y))))
   (cross-product (iota 3 (1- x))
                  (iota 3 (1- y)))))

(define total-flash-count 0)

(define (step-world! world)
  ;; (define next-world (make-array 0 12 12))
  ;; (for-each (lambda (n)
  ;;             (array-set! next-world 100 0 n)
  ;;             (array-set! next-world 100 11 n)
  ;;             (array-set! next-world 100 n 0)
  ;;             (array-set! next-world 100 n 11))
  ;;           (iota 12))
  (for-each
   (label
    loop
    (match-lambda
      ((x y)
       (define new-value
         (1+ (array-ref world y x)))
       (array-set! world #; next-world new-value y x
                                )
       (when (= 10 new-value)
         (set! total-flash-count (1+ total-flash-count))
         (let ((neighs (neighbours x y)))
          (for-each loop neighs)))
       )))
   (cross-product (iota 10 1)
                  (iota 10 1)))

  (for-each (match-lambda ((x y)
                           (when (< 9 (array-ref world y x))
                             (array-set! world 0 y x))))
            (cross-product (iota 10 1)
                           (iota 10 1)))

  world
  ; next-world
  )


(define (main args)
  (sdl-init '(video))

  (ttf-init)

  (let ()

    (define font (load-font "/usr/share/fonts/OTF/FiraMono-Medium.otf" 12))
    (define font-cache (make-vector 10))

    (define almost-world
      (->>
       (port->stream (current-input-port) read-line)
       (stream-map string->list)
       (stream-map (lambda (lst)
                     (map (compose (lambda (x) (- x (char->integer #\0)))
                                   char->integer)
                          lst)))
       stream->list
       (list->array 2)))

    (define world (make-array 100 12 12))
    (define window (make-window) )
    (define rend (make-renderer window))
    (define scale 40)
    (define framecount 0)
    (define-values (width height)
      (window-size window))

    (for-each (match-lambda ((x y)
                             (array-set! world (array-ref almost-world
                                                          y x)
                                         (1+ y) (1+ x))))
              (cross-product (iota 10)
                             (iota 10)))

    (for-each (lambda (i)
                (vector-set!
                 font-cache i
                 (call-with-surface
                  (render-font-blended font (format #f "~a" i) (make-color 0 0 0 #xFF))
                  (lambda (surface)
                    (let ((width (surface-width surface))
                          (height (surface-height surface))
                          (texture (surface->texture rend surface)))
                      (lambda (renderer x y)
                        (render-copy renderer texture
                                     #:dstrect (list x y width height))))))))
              (iota 10))

    (while #t
      (set-renderer-draw-color! rend 0 0 0 #xFF)
      (clear-renderer rend)
      (for-each (match-lambda
                  ((x y)
                   (define x-offset (- (/ width 2) (* 10/2 scale)))
                   (define y-offset (- (/ height 2) (* 10/2 scale)))
                   (let ((c (floor (* 255 (/ (array-ref world (1+ y) (1+ x)) 9)))))
                     ;; (format #t "y = ~a, x = ~a, c = ~a~%" (1+ y) (1+ x) c)
                     (set-renderer-draw-color! rend c c c #xFF))
                   (fill-rect rend (make-rect (+ x-offset (* x scale))
                                              (+ y-offset (* y scale))
                                              scale scale))
                   ((vector-ref font-cache (array-ref world (1+ y) (1+ x)))
                    rend (+ x-offset (* scale x)) (+ y-offset (* scale y)))
                   ))
                (cross-product (iota 10)
                               (iota 10)))
      (call-with-surface (render-font-blended font (format #f "~a" framecount)
                                              (make-color #xFF #xFF #xFF #xFF))
                         (lambda (surface)
                           (define texture (surface->texture rend surface))
                           (render-copy rend texture #:dstrect (list 10 10
                                                                          (surface-width surface)
                                                                          (surface-height surface)))
                           (delete-texture! texture)))
      (present-renderer rend)
      ;; (usleep 100000 )
      (set! world (step-world! world))
      (set! framecount (1+ framecount))
      (when (= 100
               (count (match-lambda ((x y) (zero? (array-ref world y x))))
                      (cross-product (iota 10 1)
                                     (iota 10 1))))
        (format #t "~a: flash count = ~a~%"
                framecount total-flash-count)
        (break))
      (let loop ((event (poll-event)))
        (when event
          (cond ((quit-event? event) (break))
                ((keyboard-down-event? event)

                 #;
                 (when (= 100 )
                   (break))
                 #;
                 (when (eq? 'space (keyboard-event-key event))
                   (set! world (step-world! world))
                   (set! framecount (1+ framecount)))))
          (loop (poll-event)))))

    (delete-renderer! rend)
    (close-window! window)))

