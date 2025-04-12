// Tab switching functionality
        function opentab(tabname) {
            var tablinks = document.getElementsByClassName("tab-links1");
            var tabcontents = document.getElementsByClassName("tab-contents");

            for (var tablink of tablinks) {
                tablink.classList.remove("active-link");
            }

            for (var tabcontent of tabcontents) {
                tabcontent.classList.remove("active-tab");
                // Reset animations
                tabcontent.style.animation = 'none';
                tabcontent.offsetHeight; // Force reflow
                tabcontent.style.animation = null;
            }

            const selectedTab = document.getElementById(tabname);
            selectedTab.classList.add("active-tab");
            event.currentTarget.classList.add("active-link");

            if (tabname === 'languages') {
                // Reset language item animations
                const languageItems = selectedTab.querySelectorAll('.language-item');
                languageItems.forEach((item, index) => {
                    item.style.animation = 'none';
                    item.offsetHeight; // Force reflow
                    item.style.animation = `slideIn 0.5s cubic-bezier(0.4, 0, 0.2, 1) forwards ${index * 0.1}s`;
                });
                animateProgressBars();
            }
        }

        // Smooth text animation
        function printText(element, text, index = 0) {
            if (index < text.length) {
                element.innerHTML += text.charAt(index);
                index++;
                setTimeout(function() {
                    printText(element, text, index);
                }, 10);
            }
        }

        // Initialize text animation
        window.onload = function() {
            const outputElement = document.getElementById('textToPrint');
            const originalText = outputElement.innerText;
            outputElement.innerHTML = '';
            printText(outputElement, originalText);
        }

        // Header scroll effect
        window.addEventListener('scroll', function() {
            const header = document.querySelector('header');
            if (window.scrollY > 50) {
                header.classList.add('scrolled');
            } else {
                header.classList.remove('scrolled');
            }
        });

        // Active navigation link handling
        document.addEventListener('DOMContentLoaded', function() {
            const sections = document.querySelectorAll('section');
            const navLinks = document.querySelectorAll('nav ul li a');

            window.addEventListener('scroll', () => {
                let current = '';
                sections.forEach(section => {
                    const sectionTop = section.offsetTop;
                    const sectionHeight = section.clientHeight;
                    if (scrollY >= (sectionTop - sectionHeight / 3)) {
                        current = section.getAttribute('id');
                    }
                });

                navLinks.forEach(link => {
                    link.classList.remove('active');
                    if (link.getAttribute('href').slice(1) === current) {
                        link.classList.add('active');
                    }
                });
            });
        });

        // Add at the beginning of the script section
        function toggleTheme() {
            const html = document.documentElement;
            const themeIcon = document.querySelector('.theme-toggle i');
            
            if (html.getAttribute('data-theme') === 'dark') {
                html.setAttribute('data-theme', 'light');
                themeIcon.className = 'bx bx-sun';
                localStorage.setItem('theme', 'light');
            } else {
                html.setAttribute('data-theme', 'dark');
                themeIcon.className = 'bx bx-moon';
                localStorage.setItem('theme', 'dark');
            }
        }

        // Initialize theme from localStorage
        document.addEventListener('DOMContentLoaded', function() {
            const savedTheme = localStorage.getItem('theme') || 'dark';
            const themeIcon = document.querySelector('.theme-toggle i');
            
            document.documentElement.setAttribute('data-theme', savedTheme);
            themeIcon.className = savedTheme === 'dark' ? 'bx bx-moon' : 'bx bx-sun';

            // ... rest of your existing DOMContentLoaded code ...
        });

        // Enhanced progress bar animation
        function animateProgressBars() {
            const progressBars = document.querySelectorAll('.progress');
            progressBars.forEach(bar => {
                const width = bar.style.width;
                bar.style.width = '0';
                // Force reflow
                bar.offsetHeight;
                setTimeout(() => {
                    bar.style.width = width;
                }, 50);
            });
        }