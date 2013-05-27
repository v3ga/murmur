//--------------------------------------------------------------
function getTheme(){ return THEME_ARBORESCENCE; }

//--------------------------------------------------------------
function setup()
{
	// Pas besoin de le déclarer «avant le setup»
	// Js est un langage de nature dynamique
	this.listParticles = Array();
	this.distParticles = 40; // pixels
}

//--------------------------------------------------------------
function draw(w,h)
{
	of.Background(0,0,0);
	of.SetColor(255);
	of.SetLineWidth(3);


	// Le classique des classiques :)
	var pi,pj;
	var i,j;
	for (i=0; i<this.listParticles.length; i++ )
		for (j=i+1; j<this.listParticles.length; j++ )
		{
			pi = this.listParticles[i];
			pj = this.listParticles[j];
			if (of.Dist(pi.x,pi.y,pj.x,pj.y)<=this.distParticles )
			{
				of.Line( pi.x,pi.y,pj.x,pj.y );
			}
	}

	of.SetLineWidth(1);
}

//--------------------------------------------------------------
function update(dt)
{
	var p;
	// Important de parcourir la liste en arrière à cause du «splice»
	for (var i=this.listParticles.length-1; i>=0 ; i-- )
	{
		// Référence sur la particule
		p = this.listParticles[i];
		// Mise à jour de la particule
		p.update(dt);
		// Trop vieille ? On l'enlève du tableau !
		if (p.isTooOld()){
			this.listParticles.splice(i,1);
		}
	}
}

//--------------------------------------------------------------
function onNewPacket(deviceId,volume,x,y)
{
	if (volume>=0.75)
	{
		// écriture équivalente en Java/Processing à this.listParticles.add( new Particle2D(x,y)  )
		// si «listParticles» était une ArrayList
		this.listParticles.push( new Particle2D(x,y) );  	
	}
}
