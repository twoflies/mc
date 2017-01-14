#ifndef EVENT_H_
#define EVENT_H_

// Event

#include <vector>
#include <algorithm>

template<typename D>
class EventHandler_ {
  public:
  virtual void handleEvent(const D *data) = 0;
};

template<class T, typename D>
class EventHandler : public EventHandler_<D> {
 public:
  EventHandler(T *t, void (T::*f)(const D*)) {
    t_ = t;
    f_ = f;
  }
  virtual ~EventHandler() {}

  void handleEvent(const D *data) {
    (t_->*f_)(data);
  }

 private:
  T *t_;
  void (T::*f_)(const D*);
};

template<typename D>
class Event {
 public:
  Event() {};
  virtual ~Event() {};
  void addHandler(EventHandler_<D> *handler) {
    handlers_.push_back(handler);
  }
  void removeHandler(EventHandler_<D> *handler) {
    handlers_.erase(std::remove(handlers_.begin(), handlers_.end(), handler), handlers_.end());
  }
  void fire(const D *data) {
    for(typename std::vector<EventHandler_<D>*>::iterator it = handlers_.begin(); it != handlers_.end(); ++it) {
      (*it)->handleEvent(data);
    }
  }

 private:
  std::vector<EventHandler_<D>*> handlers_;
};

#endif // EVENT_H_
