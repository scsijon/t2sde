-- --- T2-COPYRIGHT-NOTE-BEGIN ---
-- This copyright note is auto-generated by ./scripts/Create-CopyPatch.
-- 
-- T2 SDE: misc/luabash/md5/md5.lua
-- Copyright (C) 2006 The T2 SDE Project
-- Copyright (C) 2005 Roberto Ierusalimschy
-- 
-- More information can be found in the files COPYING and README.
-- 
-- This program is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation; version 2 of the License. A copy of the
-- GNU General Public License can be found in the file COPYING.
-- --- T2-COPYRIGHT-NOTE-END ---

----------------------------------------------------------------------------
-- $Id: md5.lua,v 1.4 2006/08/21 19:24:21 carregal Exp $
----------------------------------------------------------------------------

local core = require"md5.core"
local string = require"string"

module ("md5")

----------------------------------------------------------------------------
-- @param k String with original message.
-- @return String with the md5 hash value converted to hexadecimal digits

function sum (k)
  return (string.gsub(core.dosum(k), ".", function (c)
           return string.format("%02x", string.byte(c))
         end))
end
