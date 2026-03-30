var mRobot = require('node-gyp-build')(__dirname);

//----------------------------------------------------------------------------//
// Module.Segment                                                             //
//----------------------------------------------------------------------------//

function Segment()
{
	// Auto instantiate the Segment
	if (!(this instanceof Segment))
		return new Segment();

	this.valid = false;
	this.base  = 0;
	this.size  = 0;
	this.name  = "";
}

Segment.prototype.contains = function (value)
{
	if (typeof value === "number")
	{
		var base = this.base;
		var stop = this.base +
				   this.size;

		return base <= value &&
			   stop >  value;
	}

	throw new TypeError ("Invalid arguments");
};

Segment.prototype.lt = function (value)
{
	if (value instanceof Segment)
		return this.base < value.base;

	if (typeof value === "number")
		return this.base < value;

	throw new TypeError ("Invalid arguments");
};

Segment.prototype.gt = function (value)
{
	if (value instanceof Segment)
		return this.base > value.base;

	if (typeof value === "number")
		return this.base > value;

	throw new TypeError ("Invalid arguments");
};

Segment.prototype.le = function (value)
{
	if (value instanceof Segment)
		return this.base <= value.base;

	if (typeof value === "number")
		return this.base <= value;

	throw new TypeError ("Invalid arguments");
};

Segment.prototype.ge = function (value)
{
	if (value instanceof Segment)
		return this.base >= value.base;

	if (typeof value === "number")
		return this.base >= value;

	throw new TypeError ("Invalid arguments");
};

Segment.prototype.eq = function (segment)
{
	if (!(segment instanceof Segment))
		throw new TypeError ("Invalid arguments");

	return this.valid === segment.valid
		&& this.base  === segment.base
		&& this.size  === segment.size
		&& this.name  === segment.name;
};

Segment.prototype.ne = function (segment)
{
	if (!(segment instanceof Segment))
		throw new TypeError ("Invalid arguments");

	return this.valid !== segment.valid
		|| this.base  !== segment.base
		|| this.size  !== segment.size
		|| this.name  !== segment.name;
};

Segment.prototype.clone = function()
{
	var copy = new Segment();
	copy.valid = this.valid;
	copy.base  = this.base;
	copy.size  = this.size;
	copy.name  = this.name;
	return copy;
};

mRobot.Module.Segment = Segment;



//----------------------------------------------------------------------------//
// Module.prototype.getSegments                                               //
//----------------------------------------------------------------------------//

var _origGetModules = mRobot.Process.prototype.getModules;
mRobot.Process.prototype.getModules = function (regex)
{
	var modules = _origGetModules.call(this, regex);
	for (var i = 0; i < modules.length; i++)
	{
		modules[i]._segments = null;
		modules[i]._proc = this;
	}
	return modules;
};

mRobot.Module.prototype.getSegments = function()
{
	if (!this.valid) return [];

	if (this._segments === null)
	{
		var rawSegs = mRobot.Process._getSegments(this._proc, this.base);
		this._segments = [];
		for (var i = 0; i < rawSegs.length; i++)
		{
			var s = new Segment();
			s.valid = rawSegs[i].valid;
			s.base  = rawSegs[i].base;
			s.size  = rawSegs[i].size;
			s.name  = rawSegs[i].name;
			this._segments.push(s);
		}
	}

	return this._segments;
};



//----------------------------------------------------------------------------//
// Clone                                                                      //
//----------------------------------------------------------------------------//

mRobot.Hash.prototype.clone = function()
{
	var copy = new mRobot.Hash();
	copy.result = this.result;
	return copy;
};

mRobot.Color.prototype.clone = function()
{
	return new mRobot.Color (this);
};

mRobot.Image.prototype.clone = function()
{
	return new mRobot.Image (this);
};

mRobot.Range.prototype.clone = function()
{
	return new mRobot.Range (this);
};

mRobot.Point.prototype.clone = function()
{
	return new mRobot.Point (this);
};

mRobot.Size.prototype.clone = function()
{
	return new mRobot.Size (this);
};

mRobot.Bounds.prototype.clone = function()
{
	return new mRobot.Bounds (this);
};

mRobot.Keyboard.prototype.clone = function()
{
	var copy = new mRobot.Keyboard();
	copy.autoDelay = this.autoDelay.clone();
	return copy;
};

mRobot.Mouse.prototype.clone = function()
{
	var copy = new mRobot.Mouse();
	copy.autoDelay = this.autoDelay.clone();
	return copy;
};

mRobot.Process.prototype.clone = function()
{
	return new mRobot.Process (this.getPID());
};

mRobot.Module.prototype.clone = function()
{
	var copy = new mRobot.Module (this);

	if (this._segments !== null && this._segments !== undefined)
	{
		copy._segments = [];
		this._segments.map (function (s)
		{
			copy._segments.push (s.clone());
		});
	}
	else
	{
		copy._segments = null;
	}

	return copy;
};

mRobot.Memory.prototype.clone = function()
{
	return new mRobot.Memory (this.getProcess());
};

mRobot.Memory.Stats.prototype.clone = function()
{
	var copy = new mRobot.Memory.Stats();

	copy.systemReads  = this.systemReads;
	copy.cachedReads  = this.cachedReads;
	copy.systemWrites = this.systemWrites;
	copy.accessWrites = this.accessWrites;

	copy. readErrors  = this. readErrors;
	copy.writeErrors  = this.writeErrors;

	return copy;
};

mRobot.Memory.Region.prototype.clone = function()
{
	var copy = new mRobot.Memory.Region();

	copy.valid      = this.valid;
	copy.bound      = this.bound;

	copy.start      = this.start;
	copy.stop       = this.stop;
	copy.size       = this.size;

	copy.readable   = this.readable;
	copy.writable   = this.writable;
	copy.executable = this.executable;
	copy.access     = this.access;

	copy["private"] = this["private"];
	copy.guarded    = this.guarded;

	return copy;
};

mRobot.Window.prototype.clone = function()
{
	return new mRobot.Window (this.getHandle());
};

mRobot.Screen.prototype.clone = function()
{
	return new mRobot.Screen (this.getBounds().clone(), this.getUsable().clone());
};

mRobot.Timer.prototype.clone = function()
{
	var copy = new mRobot.Timer (this);
	return copy;
};



//----------------------------------------------------------------------------//
// Compare                                                                    //
//----------------------------------------------------------------------------//

mRobot.Timer.compare = function (a, b)
{
	if (a.lt (b)) return -1;
	if (a.gt (b)) return  1;
	return 0;
};

mRobot.Module.compare = function (a, b)
{
	if (a.lt (b)) return -1;
	if (a.gt (b)) return  1;
	return 0;
};

mRobot.Module.Segment.compare = function (a, b)
{
	if (a.lt (b)) return -1;
	if (a.gt (b)) return  1;
	return 0;
};

mRobot.Memory.Region.compare = function (a, b)
{
	if (a.lt (b)) return -1;
	if (a.gt (b)) return  1;
	return 0;
};



//----------------------------------------------------------------------------//
// Exports                                                                    //
//----------------------------------------------------------------------------//

module.exports = mRobot;
