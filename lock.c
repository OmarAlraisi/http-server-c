struct int_lock {
  /// if 0, the value is unlocked and thread can aquire the lock, otherwise,
  /// thread has to wait till the loack is free again
  unsigned short lock;

  /// the value behind the lock
  int val;
};

void aquire(struct int_lock *lock) {
  while (lock->lock != 0) {
    // block until free
  }
  lock->lock = 1;
}

void release(struct int_lock *lock) { lock->lock = 0; }

int increment_and_get_val(struct int_lock *lock) {
  ++(lock->val);
  return lock->val;
}
