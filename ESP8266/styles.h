const char _styles[] PROGMEM = R"=====(
:root {
	--background: #393E46;
	--title: #EEEEEE;
	--header: #222831;
	--g-btn: #EEEEEE;
    --g-btn-hover: #0ff0ff;
	--navbar: #222831;
	--nav-tile: #EEEEEE;
	--nav-tile-hover: #393E46;
	--gamepad-button: #7BC74D;
	--headLight-glow: #0ff0ff;
	--text: #EEEEEE;
	--dot-online: #81B214;
	--dot-offline: #FF2626;
	--dot-reconn: #FFCC29;
	--card: #2B2B2B;
	--card-border: #3f4650;
	--card-btn: #602080;
	--fontSizeLarge : 1.5em;
	--fontSizeSmall : 1.2em;
}

* {
	box-sizing: border-box;
	padding: 0;
	margin: 0;
	outline: none;
	-webkit-touch-callout: none;
    -webkit-user-select: none; 
    -khtml-user-select: none; 
    -moz-user-select: none; 
    -ms-user-select: none; 
    user-select: none;
}
body{
    background-color: var(--background);
}
.container {  
	font-family: Calibri, 'Gill Sans', 'Gill Sans MT', 'Trebuchet MS', sans-serif;
	width: 100vw;
	min-height: 100vh;
}

.row {
	display: flex;
	justify-content: space-between;
	vertical-align: middle;
}

.c3 {
	width: 33.33%;
	padding: 4px;
}
.t-center{
	text-align: center;
}
.px{
	padding-right: 15px;
	padding-left: 15px;
}
.py{
	padding-top: 15px;
	padding-bottom: 15px;
}
.mx{
	margin-right: 15px;
	margin-left: 15px;
}
.mt{
	margin-top: 15px;
}
.my{
	margin-top: 15px;
	margin-bottom: 15px;
}
.mx-2{
	margin-right: 2em;
	margin-left: 2em;
}
.my-2{
	margin-top: 2em;
	margin-bottom: 2em;
}
.w-50{
	width: 50%;
}

.w-100{
	width: 100%;
}
.c-pointer{
	cursor: pointer;
}
.df-c{
	display: flex;
	flex-direction: column;
	justify-content: center;
}
.dfe{
	display: flex;
	justify-content: space-evenly;
}
.d-n{
	display: none;
}
.dot{
	margin: 12px 0 0 6px;
	display: table;
	border-radius: 50%;
	height : 12px;
	width : 12px;
}
.dot-offline{
	background-color: var(--dot-offline);
}
.dot-online{
	background-color: var(--dot-online);
}
.dot-reconn{
	background-color: var(--dot-reconn);
}
/*header*/

.header {
	background-color: var(--header);
}


/*nav bar*/

.nav-icon {
	display: flex;
	justify-content: flex-start;
	vertical-align: middle;
	width: 33.33%;
}

.nav-hamburger {
	background: none;
	border: none;
	margin-left: 10px;
}

.nav-hamburger svg {
	stroke: var(--title);
	width: 2em;
	height: 2em;	
}

.nav-bar {
	position: absolute;
	display: none;
	flex-direction: column;
	background-color: var(--navbar);
	width: 25vw;
	height: calc(100vh - 30px);
	top: 0;
	left: 0;
	transform: translateX(-100%);
}


.nav-tiles {
	display: flex;
	justify-content: center;
	padding: 15px;
	width: 100%;
	color: var(--nav-tile);
    font-size: var(--fontSizeLarge);
}
.nav-tiles svg{
	width: 0.9em;
	height: 0.9em;
	margin-left: 5px;
	stroke: var(--gamepad-button);
	stroke-width: 2;
	display: none;
}
.push-down{
	cursor: pointer;
}
.push-down:active{
	transform:scale(0.96);
}

#headlight:hover{
	background-color: var(--navbar);
}


.nav-tiles:hover{
    background-color: var(--nav-tile-hover);
}

/* Title */

.title {
	text-align: center;
	color: var(--title);
	font-size: 1.7em;
	padding: 4px;
}


/* button */

.g-btn {
	width: 16vh;
    height: 16vh;
	margin: 10px;
	border: none;
	background-color: var(--g-btn);
    transition: 0.2s box-shadow ease-in;
}
.g-btn:hover{
    box-shadow: 0 0 20px 3px var(--g-btn-hover);
}
.g-btn:active{
    transform: scale(0.96);
}
.g-btn svg{
    width: 8vh;
    height: 8vh;
    stroke: var(--gamepad-button);
    stroke-width: 2;
}

.forward {
	border-radius: 50% 50% 10% 10%;
}

.backward {
	border-radius: 10% 10% 50% 50%;
}

.left {
	border-radius: 50% 10% 10% 50%;
}

.right {
	border-radius: 10% 50% 50% 10%;
}

.stop {
	border-radius: 50%;
}


/* content */

.content {
	position: relative;
}


/* gamepad */

.gamepad {
	padding: 3em;
	display: flex;
	flex-direction: column;
}

.row-center {
	display: flex;
	justify-content: center;
	vertical-align: middle;
}
 /* Sliders */

 .slider {
    -webkit-appearance: none;
    height: 0.8em;
    border-radius: 5px;
    background: var(--g-btn);
    outline: none;
    transition: filter 0.1s ease-in-out;
  }
  
  .slider::-webkit-slider-thumb {
    -webkit-appearance: none;
    appearance: none;
    width: 2em;
    height: 2em;
    border-radius: 50%;
    background: var(--gamepad-button);;
    cursor: pointer;
  }
  
  .slider::-moz-range-thumb {
    width: 2em;
    height: 2em;
    border-radius: 50%;
    background:var(--gamepad-button);;
    cursor: pointer;
  }
  
  .speed{
	width: 50%;
	margin: 5px;
  }
  .text{
	color: var(--text);
  }

  /* Status Page */
  .status-page{
	  padding : 1.5em;
	  flex-direction: column;
	  justify-content: flex-start;
	  color : var(--text);
	  font-size: var(--fontSizeLarge);
  }
  .status-page > p{
	  margin-top: 5px;
	  text-align: center;
  }
  
  /*setup page*/

  .setup{
	  display: none;
	  justify-content: space-around;
	  padding: 2em;
  }
  /* card */
  .card{
	  display: flex;
	  flex-direction: column;
	  background-color: var(--card);
	  padding: 1em;
	 
	  border-radius: 4px;
	  font-size: var(--fontSizeLarge);	  
	  border: 2px solid var(--card-border);
	  box-shadow: 0 0 20px 3px var(--card-border);
  }
  .card-btn{
	padding: 10px;
	background-color: var(--card-btn);
	color: var(--text);
	cursor: pointer;
	border-radius: 4px;
	border:none;
	font-size: 15px;
  }
  .card-btn:active{
	  transform: scale(0.96);
  }
  .card > div{
	margin: 10px 0;
   }
   .card-textbox{
	   border: none;
	   border-radius: 4px;
	   padding: 12px;
   }

   /* reset page */
   .reset-page{
	   display: none;
	   justify-content: space-around;
	   padding: 2em;
   }
@media only screen and (max-width: 600px) {
	/* navbar */
	.nav-bar {
		width: 50vw;
	}
	.dot{
		margin: 7px 0 0 6px;
		display: table;
		border-radius: 50%;
		height : 12px;
		width : 12px;
	}
	/* gamepad */
	.g-btn{
		width: 18vw;
		height: 18vw;
		transition: 0s box-shadow;
	}
	.g-btn:hover{
		box-shadow: 0 0;
	}
	.g-btn svg{
		width: 8vw;
		height: 8vw;
	}
	
	.speed{
		width: 90%;
	}
	.status-page{
		font-size: var(--fontSizeSmall);
		padding: 0.8em;
	}
	/* status page */
	.status-page > p{
		text-align: end;
	}
	.nav-tiles {
		font-size: var(--fontSizeSmall);
	}

	/* Slider */
	.slider {
		height: 0.7em;
	  }
	  
	  .slider::-webkit-slider-thumb {
		width: 1.7em;
		height: 1.7em;
	  }
	  
	  .slider::-moz-range-thumb {
		width: 1.7em;
		height: 1.7em;
	  }
	
	  /* setup */
	  .setup{
		  flex-direction: column;
		  justify-content: flex-start;
	  }
	  .card{
		  font-size: 20px;
		  padding: 10px;
		  margin-bottom: 15px;
	  }
	  .card > div{
		  margin: 8px 0;
	  }
	  /* Reset page */
	  .reset-page{
		  flex-direction: column;
		  justify-content: flex-start;
	  }
}
)=====";