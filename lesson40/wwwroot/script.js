function startAnimation() {
    const img = document.getElementById("slideImage");
  
    // 移除旧的动画类，保证可以重复播放
    img.classList.remove("animate");
  
    // 强制重排以重新触发动画
    void img.offsetWidth;
  
    // 添加动画类
    img.classList.add("animate");
  }